// Developed by Ivan Piankouski. All Rights Reserved


#include "PlayerInteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractableComponent.h"
#include "Components/ShapeComponent.h"
#include "Interfaces/InteractableInterface.h"
#include "Interfaces/InteractorInterface.h"


UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInteractionComponent::BindInteractionAction()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn)
	{
		return;
	}

	APlayerController* PlayerController = OwningPawn->GetController<APlayerController>();
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	if (InputSubsystem && EnhancedInputComponent)
	{
		InputSubsystem->AddMappingContext(InteractionMappingContext, InteractionPriority);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::InteractAction_Started);
	}
}

void UPlayerInteractionComponent::SetInteractionCollision(UShapeComponent* InInteractionCollision)
{
	if (InteractionCollision == InInteractionCollision)
	{
		return;
	}

	// Unbinds from the current collision's delegates
	if (InteractionCollision)
	{
		InteractionCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnInteractableBeginOverlap);
		InteractionCollision->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnInteractableEndOverlap);
	}

	// Sets new collision and binds to its delegates
	InteractionCollision = InInteractionCollision;
	if (InteractionCollision)
	{
		InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractableBeginOverlap);
		InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractableEndOverlap);
	}
}

AActor* UPlayerInteractionComponent::GetFocusedInteractableActor() const
{
	if(FocusedInteractableActor.IsValid())
	{
		return FocusedInteractableActor.Get();
	}
	return nullptr;
}

void UPlayerInteractionComponent::OnRegister()
{
	Super::OnRegister();

	// Ensures that owner implements interactor interface 
	ensureMsgf(GetOwner()->Implements<UInteractorInterface>(),
		TEXT("%s of %s class has UPlayerInteractionComponent but does not implement IInteractorIterface"),
		*GetNameSafe(GetOwner()),
		*GetOwner()->GetClass()->GetName());
}

bool UPlayerInteractionComponent::CanInteract() const
{
	bool bCanOwnerInteract = false;
	bool bCanFocusedActorInteract = false;

	// Checks if the owner can interact
	if (GetOwner()->Implements<UInteractorInterface>())
	{
		bCanOwnerInteract = IInteractorInterface::Execute_CanInteract(GetOwner());
	}
	// Checks if the interactable actor can interact
	if(FocusedInteractableActor.IsValid())
	{
		bCanFocusedActorInteract = IInteractableInterface::Execute_CanInteract(FocusedInteractableActor.Get(), GetOwner());
	}
	
	return bCanOwnerInteract && bCanFocusedActorInteract;
}

AActor* UPlayerInteractionComponent::GetClosestInteractableActor() const
{
	if (AvailableInteractableActors.IsEmpty())
	{
		return nullptr;
	}

	float ClosestDistance = FLT_MAX;
	AActor* ClosestInteractableActor = AvailableInteractableActors[0].IsValid() ? AvailableInteractableActors[0].Get() : nullptr;

	for (TWeakObjectPtr<AActor> InteractableActor : AvailableInteractableActors)
	{
		if (!InteractableActor.IsValid())
		{
			continue;
		}

		// Compares the cached distance with the current distance and caches the closest and the actor
		const float DistanceToInteractable = FVector::Distance(GetOwner()->GetActorLocation(), InteractableActor->GetActorLocation());
		if (DistanceToInteractable < ClosestDistance)
		{
			ClosestDistance = DistanceToInteractable;
			ClosestInteractableActor = InteractableActor.Get();
		}
	}

	return ClosestInteractableActor;
}

void UPlayerInteractionComponent::UpdateFocusedInteractableActor()
{
	AActor* ClosestInteractableActor = GetClosestInteractableActor();
	
	if(ClosestInteractableActor == FocusedInteractableActor)
	{
		return;
	}
	
	if(FocusedInteractableActor.IsValid())
	{
		SetInteractableActorFocused(FocusedInteractableActor.Get(), false);
	}
	
	FocusedInteractableActor = ClosestInteractableActor;
	if(FocusedInteractableActor.IsValid())
	{
		SetInteractableActorFocused(FocusedInteractableActor.Get(), true);
	}
}

void UPlayerInteractionComponent::PerformInteraction() const
{
	if(!FocusedInteractableActor.IsValid())
	{
		return;
	}
	
	if (CanInteract())
	{
		IInteractableInterface::Execute_Interact(FocusedInteractableActor.Get(), GetOwner());
	}
	else
	{
		IInteractableInterface::Execute_OnFailedInteract(FocusedInteractableActor.Get(), GetOwner());
	}
}

void UPlayerInteractionComponent::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent,
															 AActor* OtherActor, UPrimitiveComponent* OtherComp,
															 int32 OtherBodyIndex, bool bFromSweep,
															 const FHitResult& SweepResult)
{
	// Adds to the list actors that implements IinteractableInterface
	if (OtherActor && OtherActor->Implements<UInteractableInterface>())
	{
		AddActorToInteractableActors(OtherActor);
	}
}

void UPlayerInteractionComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
														   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		RemoveActorFromInteractableActors(OtherActor);
	}
}

void UPlayerInteractionComponent::AddActorToInteractableActors(AActor* InActor)
{
	if(InActor)
	{
		AvailableInteractableActors.AddUnique(InActor);

		UpdateFocusedInteractableActor();

		// Enable updating the closest actor, if the list num is greater than 1
		if (AvailableInteractableActors.Num() > 1 && GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(UpdateInteractableTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(UpdateInteractableTimerHandle, this, &ThisClass::UpdateFocusedInteractableActor, UpdateFocusedActorRate, true);
		}
	}
}

void UPlayerInteractionComponent::RemoveActorFromInteractableActors(const AActor* InActor)
{
	if(!InActor)
	{
		return;
	}

	// Finds removing actor's index
	const int32 ActorToRemoveIndex = AvailableInteractableActors.IndexOfByPredicate([InActor](const TWeakObjectPtr<AActor> InteractableActor)
	{
		return InteractableActor == InActor;
	});

	if(AvailableInteractableActors.IsValidIndex(ActorToRemoveIndex))
	{
		AvailableInteractableActors.RemoveAt(ActorToRemoveIndex);
	}
	
	UpdateFocusedInteractableActor();

	// Stops updating the closest actor, if nothing overlaps 
	if (AvailableInteractableActors.Num() <= 1 && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateInteractableTimerHandle);
	}
}

void UPlayerInteractionComponent::InteractAction_Started(const FInputActionValue& InputActionValue)
{
	PerformInteraction();
}

void UPlayerInteractionComponent::SetInteractableActorFocused(const AActor* InActor, bool bFocused)
{
	if(!InActor)
	{
		return;
	}

	if(UInteractableComponent* InteractableComponent = IInteractableInterface::Execute_GetInteractableComponent(InActor))
	{
		InteractableComponent->SetIsFocused(bFocused);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


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

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	if (InputSubsystem && EnhancedInputComponent)
	{
		InputSubsystem->AddMappingContext(InteractionMappingContext, InteractionPriority);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
		                                   &ThisClass::InteractAction_Started);
	}
}

void UPlayerInteractionComponent::SetInteractionCollision(UShapeComponent* InInteractionCollision)
{
	if (InteractionCollision == InInteractionCollision)
	{
		return;
	}

	if (InteractionCollision)
	{
		InteractionCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnInteractableBeginOverlap);
		InteractionCollision->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnInteractableEndOverlap);
	}

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

bool UPlayerInteractionComponent::CanInteract() const
{
	if (GetOwner()->Implements<UInteractorInterface>())
	{
		return IInteractorInterface::Execute_CanInteract(GetOwner());
	}

	return true;
}

AActor* UPlayerInteractionComponent::GetClosestInteractableActor() const
{
	if (AvailableInteractableActors.IsEmpty())
	{
		return nullptr;
	}

	float ClosestDistance = FLT_MAX;
	AActor* ClosestInteractableActor = nullptr;

	for (TWeakObjectPtr<AActor> InteractableActor : AvailableInteractableActors)
	{
		if (!InteractableActor.IsValid())
		{
			continue;
		}

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
	
	UInteractableComponent* ClosestInteractableComponent = ClosestInteractableActor
	?  ClosestInteractableActor->GetComponentByClass<UInteractableComponent>()
	: nullptr;
	
	UInteractableComponent* FocusedInteractableComponent = FocusedInteractableActor.IsValid()
	? FocusedInteractableActor->GetComponentByClass<UInteractableComponent>()
	: nullptr;

	if(FocusedInteractableComponent)
	{
		FocusedInteractableComponent->SetIsFocused(false);
	}
	
	FocusedInteractableComponent = ClosestInteractableComponent;
	if(FocusedInteractableComponent)
	{
		FocusedInteractableComponent->SetIsFocused(true);
	}

	FocusedInteractableActor = ClosestInteractableActor;
}

void UPlayerInteractionComponent::PerformInteraction() const
{
	if (!CanInteract() || !FocusedInteractableActor.IsValid())
	{
		return;
	}

	if (UInteractableComponent* FocusedInteractableComponent = FocusedInteractableActor->GetComponentByClass<UInteractableComponent>())
	{
		FocusedInteractableComponent->Interact(GetOwner());
		return;
	}
	
	if (FocusedInteractableActor->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_Interact(FocusedInteractableActor.Get(), GetOwner());
	}
}

void UPlayerInteractionComponent::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent,
															 AActor* OtherActor, UPrimitiveComponent* OtherComp,
															 int32 OtherBodyIndex, bool bFromSweep,
															 const FHitResult& SweepResult)
{
	if (OtherActor &&
		(OtherActor->GetComponentByClass<UInteractableComponent>() || OtherActor->Implements<UInteractableInterface>()))
	{
		AddActorToInteractableActor(OtherActor);
	}
}

void UPlayerInteractionComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
														   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		RemoveActorInteractableActor(OtherActor);
	}
}

void UPlayerInteractionComponent::AddActorToInteractableActor(AActor* InActor)
{
	if(InActor)
	{
		AvailableInteractableActors.AddUnique(InActor);

		UpdateFocusedInteractableActor();

		if (AvailableInteractableActors.Num() > 1 && GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(UpdateInteractableTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(UpdateInteractableTimerHandle, this, &ThisClass::UpdateFocusedInteractableActor, 0.1f, true);
		}
	}
}

void UPlayerInteractionComponent::RemoveActorInteractableActor(const AActor* InActor)
{
	if(!InActor)
	{
		return;
	}

	const int32 InfoToRemoveIndex = AvailableInteractableActors.IndexOfByPredicate([InActor](const TWeakObjectPtr<AActor> InteractableActor)
	{
		return InteractableActor == InActor;
	});
	
	if(AvailableInteractableActors.IsValidIndex(InfoToRemoveIndex))
	{
		AvailableInteractableActors.RemoveAt(InfoToRemoveIndex);
	}

	UpdateFocusedInteractableActor();
	
	if (AvailableInteractableActors.Num() <= 1 && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateInteractableTimerHandle);
	}
}

void UPlayerInteractionComponent::InteractAction_Started(const FInputActionValue& InputActionValue)
{
	PerformInteraction();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CoreTypes/FPCustomCollisions.h"
#include "Interfaces/InteractableInterface.h"


void UPlayerInteractionComponent::BindInteractionAction()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if(!OwningPawn)
	{
		return;
	}

	APlayerController* PlayerController = OwningPawn->GetController<APlayerController>();
	if(!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	if(InputSubsystem && EnhancedInputComponent)
	{
		InputSubsystem->AddMappingContext(InteractionMappingContext, InteractionPriority);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::InteractAction_Started);
	}
}

void UPlayerInteractionComponent::InteractAction_Started(const FInputActionValue& InputActionValue)
{
	if(UWorld* World = GetWorld())
	{
		TArray<FHitResult> OutResults;
		
		FVector StartTrace = GetOwner()->GetActorLocation();
		FVector EndTrace = StartTrace + GetOwner()->GetActorForwardVector() * TraceLength;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		
		World->SweepMultiByObjectType(OutResults, StartTrace, EndTrace, FQuat::Identity, ECC_FP_Interactable_OC, FCollisionShape::MakeSphere(TraceRadius), QueryParams);

		TArray<TTuple<bool, float, AActor*>> InteractableActors;

		for (const FHitResult& OutResult : OutResults)
		{
			AActor* InteractableActor = OutResult.GetActor();
			if(InteractableActor && InteractableActor->Implements<UInteractableInterface>())
			{
				InteractableActors.Emplace(
					IInteractableInterface::Execute_CanInteract(InteractableActor, GetOwner()),
					FVector::DistSquared(InteractableActor->GetActorLocation(),GetOwner()->GetActorLocation()),
					InteractableActor);
			}
		}

		if(InteractableActors.IsEmpty())
		{
			return;
		}
		
		InteractableActors.Sort([](const TTuple<bool, float, AActor*>& A, const TTuple<bool, float, AActor*>& B)
		{
			if(A.Get<0>() != B.Get<0>())
			{
				return A.Get<0>();
			}
			
			return A.Get<1>() < B.Get<1>();
		});

		if(InteractableActors[0].Get<0>())
		{
			IInteractableInterface::Execute_Interact(InteractableActors[0].Get<2>(), GetOwner()); 
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractableComponent.h"
#include "Interfaces/InteractorInterface.h"


UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

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

void UPlayerInteractionComponent::AddInteractable(UInteractableComponent* InInteractableComponent)
{
	AvailableInteractables.AddUnique(InInteractableComponent);
	UpdateFocusedInteractable();

	if(AvailableInteractables.Num() > 1 && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateInteractableTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(UpdateInteractableTimerHandle, this, &ThisClass::UpdateFocusedInteractable, 0.1f, true);
	}
}

void UPlayerInteractionComponent::RemoveInteractable(UInteractableComponent* InInteractableComponent)
{
	AvailableInteractables.Remove(InInteractableComponent);
	UpdateFocusedInteractable();

	if(AvailableInteractables.Num() <= 1 && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateInteractableTimerHandle);
	}
}

bool UPlayerInteractionComponent::CanInteract() const
{
	if(GetOwner()->Implements<UInteractorInterface>())
	{
		return IInteractorInterface::Execute_CanInteract(GetOwner());
	}
	
	return true;
}

UInteractableComponent* UPlayerInteractionComponent::GetClosestInteractable() const
{
	if(AvailableInteractables.IsEmpty())
	{
		return nullptr;
	}

	float ClosestDistance = FLT_MAX;
	UInteractableComponent* ClosestInteractable = AvailableInteractables[0];
	
	for (UInteractableComponent* Interactable : AvailableInteractables)
	{
		if(!IsValid(Interactable))
		{
			continue;
		}
		
		AActor* InteractableOwner = Interactable->GetOwner();

		const float DistanceToInteractable = FVector::Distance(GetOwner()->GetActorLocation(), InteractableOwner->GetActorLocation());
		if(DistanceToInteractable < ClosestDistance)
		{
			ClosestDistance = DistanceToInteractable;
			ClosestInteractable = Interactable;
		}
	}

	return ClosestInteractable;
}

void UPlayerInteractionComponent::UpdateFocusedInteractable()
{
	UInteractableComponent* ClosestInteractable = GetClosestInteractable();
	if(ClosestInteractable != FocusedInteractable)
	{
		if(FocusedInteractable)
		{
			FocusedInteractable->SetIsFocused(false);
		}

		FocusedInteractable = ClosestInteractable;
		if(FocusedInteractable)
		{
			FocusedInteractable->SetIsFocused(true);
		}
	}
}

void UPlayerInteractionComponent::PerformInteraction() const
{
	if(!FocusedInteractable || !CanInteract())
	{
		return;
	}

	FocusedInteractable->Interact(GetOwner());
}

void UPlayerInteractionComponent::InteractAction_Started(const FInputActionValue& InputActionValue)
{
	PerformInteraction();
}

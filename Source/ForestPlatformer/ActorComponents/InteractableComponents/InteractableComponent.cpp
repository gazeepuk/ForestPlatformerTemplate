// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreTypes/CustomCollisions/FPCustomCollisions.h"
#include "Interfaces/InteractableInterface.h"


UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bFocused = false;
}

void UInteractableComponent::InitInteractableComponent(UShapeComponent* InCollision, UWidgetComponent* InWidgetComponent)
{
	SetInteractableCollision(InCollision);
	SetInteractionWidgetComponent(InWidgetComponent);
}

void UInteractableComponent::SetInteractableCollision(UShapeComponent* InCollision)
{
	if(InCollision == InteractionCollision)
	{
		return;
	}

	// Sets new interaction collision pointer and setups its properties for interactions
	InteractionCollision = InCollision;
	if(InteractionCollision)
	{
		InteractionCollision->SetCollisionObjectType(ECC_FP_Interactable_OC);
		InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractionCollision->SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);
		InteractionCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		InteractionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		InteractionCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		InteractionCollision->SetGenerateOverlapEvents(true);
	}
}

void UInteractableComponent::SetInteractionWidgetComponent(UWidgetComponent* InWidgetComponent)
{
	// Sets new interaction widget component pointer and setups its properties for interaction
	InteractionWidgetComponent = InWidgetComponent;
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(bFocused);
	}
}

void UInteractableComponent::SetIsFocused(bool bIsFocused)
{
	if(bIsFocused == bFocused)
	{
		return;
	}
	
	bFocused = bIsFocused;
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(bFocused);
	}
}

void UInteractableComponent::OnRegister()
{
	Super::OnRegister();

	// Ensures that the owner implements interactable interface
	ensureMsgf(GetOwner()->Implements<UInteractableInterface>(),
		TEXT("%s of %s calss has Interactable Component but does not implement IInteractableInterface."),
		*GetNameSafe(GetOwner()),
		*GetOwner()->GetClass()->GetName());
}

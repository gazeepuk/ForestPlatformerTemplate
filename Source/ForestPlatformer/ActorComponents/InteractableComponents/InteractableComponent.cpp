// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "PlayerInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreTypes/FPCustomCollisions.h"


UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bFocused = false;
}

void UInteractableComponent::SetInteractableCollision(UShapeComponent* InCollision)
{
	if(InCollision == InteractionCollision)
	{
		return;
	}

	if(InteractionCollision)
	{
		InteractionCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::UInteractableComponent::OnInstigatorBeginOverlap);
		InteractionCollision->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::UInteractableComponent::OnInstigatorEndOverlap);
	}

	InteractionCollision = InCollision;
	if(InteractionCollision)
	{
		InteractionCollision->SetCollisionObjectType(ECC_FP_Interactable_OC);
		InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractionCollision->SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);
		InteractionCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		InteractionCollision->SetGenerateOverlapEvents(true);
		
		InteractionCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::UInteractableComponent::OnInstigatorBeginOverlap);
		InteractionCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::UInteractableComponent::OnInstigatorEndOverlap);
	}
}

void UInteractableComponent::SetInteractionWidgetComponent(UWidgetComponent* InWidgetComponent)
{
	InteractionWidgetComponent = InWidgetComponent;
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
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

void UInteractableComponent::Interact(AActor* InInstigator)
{
	OnInteraction.Broadcast(InInstigator);
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractableComponent::OnInstigatorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor)
	{
		return;
	}

	if(UPlayerInteractionComponent* InteractionComponent = OtherActor->GetComponentByClass<UPlayerInteractionComponent>())
	{
		InteractionComponent->AddInteractable(this);
	}
}

void UInteractableComponent::OnInstigatorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor)
	{
		return;
	}

	if(UPlayerInteractionComponent* InteractionComponent = OtherActor->GetComponentByClass<UPlayerInteractionComponent>())
	{
		InteractionComponent->RemoveInteractable(this);
	}
}

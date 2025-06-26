// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreTypes/FPCustomCollisions.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);
	SetGenerateOverlapEvents(true);
	
	OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnInstigatorBeginOverlap);
	OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnInstigatorEndOverlap);
	
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(this);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	InteractionWidgetComponent->SetVisibility(false);
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	if(InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
		InteractionWidgetComponent->InitWidget();
	}
}

void UInteractableComponent::OnInstigatorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ShowInteractionWidget();
	}
}

void UInteractableComponent::OnInstigatorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		HideInteractionWidget();
	}
}

void UInteractableComponent::ShowInteractionWidget()
{
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(true);
	}
}

void UInteractableComponent::HideInteractionWidget()
{
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(false);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"

#include "ActorComponents/InteractableComponents/InteractableComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


AInteractableActorBase::AInteractableActorBase()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("IneractableComponent"));
	
	InteractableCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractableCollision"));
	InteractableCollision->SetupAttachment(GetRootComponent());
	
	InteractableWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidget"));
	InteractableWidgetComponent->SetupAttachment(GetRootComponent());
}

UInteractableComponent* AInteractableActorBase::GetInteractableComponent_Implementation() const
{
	return InteractableComponent;
}

bool AInteractableActorBase::CanInteract_Implementation(AActor* InInstigator)
{
	return true;
}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	if(bAutoInitInteractableComponent)
	{
		InteractableComponent->InitInteractableComponent(InteractableCollision, InteractableWidgetComponent);
	}
}

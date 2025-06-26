// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ObserverComponents/ObserverComponent.h"

#include "ObservableComponent.h"

UObserverComponent::UObserverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UObserverComponent::BeginPlay()
{
	Super::BeginPlay();

	InitObservables();
}

void UObserverComponent::InitObservables()
{
	for (const AActor* ObservableActor : ObservableActors)
	{
		if(UObservableComponent* ObservableComponent = ObservableActor->GetComponentByClass<UObservableComponent>())
		{
			ObservableComponent->OnObservableTriggered.AddDynamic(this, &ThisClass::OnObservableTriggered);

			if(!ObservableComponent->HasTriggered())
			{
				RemainingObservables++;
			}
		}
	}

	if(RemainingObservables <= 0)
	{
		OnAllObservablesTriggered.Broadcast();
	}
}

void UObserverComponent::OnObservableTriggered(UObservableComponent* InObservable)
{
	RemainingObservables--;

	if(RemainingObservables <= 0)
	{
		OnAllObservablesTriggered.Broadcast();
	}
}


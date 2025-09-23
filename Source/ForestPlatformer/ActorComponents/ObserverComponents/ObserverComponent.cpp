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
		if (UObservableComponent* ObservableComponent = ObservableActor->GetComponentByClass<UObservableComponent>())
		{
			ObservableComponent->OnObservableStateChanged.AddDynamic(this, &ThisClass::OnObservableStateChanged);
			Observables.FindOrAdd(ObservableComponent, ObservableComponent->IsObservableActive());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s does not have UObservableComponent. %s can't observe this actor"),
			       *GetNameSafe(ObservableActor), *GetNameSafe(GetOwner()));
		}
	}
}

void UObserverComponent::CheckObservablesState()
{
	bool bAllActive = true;

	for (TPair<UObservableComponent*, bool> Observable : Observables)
	{
		if (Observable.Key && !Observable.Key->IsObservableActive())
		{
			bAllActive = false;
			break;
		}
	}

	if (bAllActive)
	{
		if(!bCanTriggerAgain && bHasTriggered)
		{
			return;
		}
		
		OnAllObservablesActivated.Broadcast();
		bHasTriggered = true;
	}
}

void UObserverComponent::OnObservableStateChanged(UObservableComponent* InObservable, bool bObservableActive)
{
	if (InObservable && Observables.Contains(InObservable))
	{
		Observables[InObservable] = bObservableActive;
	}

	CheckObservablesState();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ObserverComponents/ObserverComponent.h"
#include "CoreTypes/ObservingTypes/ObserverConditions/ObserverCondition.h"

DEFINE_LOG_CATEGORY(LogFpObserver);

UObserverComponent::UObserverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObserverComponent::ResetHasTrigger()
{
	bHasTriggered = false;
}

void UObserverComponent::BeginPlay()
{
	Super::BeginPlay();

	InitObservables();
}

void UObserverComponent::InitObservables()
{
	for (const FObservedCondition& ObservedCondition : ObservedConditions)
	{
		if(ObservedCondition.IsValid())
		{
			// Finds ObservableComponent in observable actor
			if(UObservableComponent* ObservableComponent = ObservedCondition.ObservableActor->GetComponentByClass<UObservableComponent>())
			{
				// Checks if there is no such component in the array
				if(!ObservableToCondition.Contains(ObservableComponent))
				{
					// Adds to the array and binds the delegate
					ObservableComponent->OnObservableStateChanged.AddUniqueDynamic(this, &ThisClass::ObservableStateChanged);
					ObservableToCondition.Add(ObservableComponent, ObservedCondition.ObserverCondition);
				}
				else
				{
					UE_LOG(LogFpObserver, Warning,
						TEXT("%s observable component already contains in %s observer component"),
						*GetNameSafe(ObservedCondition.ObservableActor),
						*GetNameSafe(GetOwner()));
				}
			}
			else
			{
				UE_LOG(LogFpObserver, Error,
						TEXT("Observable %s actor does not have an observable component"),
						*GetNameSafe(ObservedCondition.ObservableActor));
			}
		}
	} 
}

void UObserverComponent::CheckObservableStates()
{
	bool bAllActive = true;

	// Checks if all observables have met their conditions
	for (TPair<UObservableComponent*, UObserverCondition*> ObservableCondition : ObservableToCondition)
	{
		UObservableComponent* ObservableComponent = ObservableCondition.Key;
		UObserverCondition* ObserverCondition = ObservableCondition.Value;
		
		if (ObservableComponent && ObserverCondition)
		{
			bool bHasConditionMet = ObserverCondition->IsConditionMet_Implementation(ObservableComponent->GetObservableState());
			if(!bHasConditionMet)
			{
				UE_LOG(LogFpObserver, Warning, TEXT("%s hasn't met condition"), *GetNameSafe(ObservableComponent->GetOwner()));
				bAllActive = false;
				break;
			}
		}
		else
		{
			bAllActive = false;
			break;
		}
	}
	
	if(!bCanTriggerAgain && bHasTriggered)
	{
		UE_LOG(LogFpObserver, Display, TEXT("Observer has already triggered. Can't trigger again"));
		return;
	}

	bConditionsMet = bAllActive;
	// Broadcasts condition checking result
	OnCheckConditions.Broadcast(bConditionsMet);
	
	if (bAllActive)
	{
		UE_LOG(LogFpObserver, Display, TEXT("All observer conditions have met"));
		bHasTriggered = true;
		// Broadcasts if all conditions have met 
		OnAllConditionsMet.Broadcast();
	}
	else
	{
		UE_LOG(LogFpObserver, Display, TEXT("Some observer conditions haven't met"));
	}
}

void UObserverComponent::ObservableStateChanged(UObservableComponent* InChangedObservable)
{
	OnObservableStateChanged.Broadcast(InChangedObservable);
	CheckObservableStates();
}

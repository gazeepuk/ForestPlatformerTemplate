// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ObserverComponents/ObserverComponent.h"

#include "ObservableComponent.h"
#include "CoreTypes/ObservingTypes/ObserverConditions/ObserverCondition.h"

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
			if(UObservableComponent* ObservableComponent = ObservedCondition.ObservableActor->GetComponentByClass<UObservableComponent>())
			{
				if(!ObservableToCondition.Contains(ObservableComponent))
				{
					ObservableComponent->OnObservableStateChanged.AddUniqueDynamic(this, &ThisClass::OnObservableStateChanged);
					ObservableToCondition.Add(ObservableComponent, ObservedCondition.ObserverCondition);
				}
				else
				{
					UE_LOG(LogTemp, Warning,
						TEXT("Observable %s observable component already contains in %s observer component"),
						*GetNameSafe(ObservedCondition.ObservableActor),
						*GetNameSafe(GetOwner()));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning,
						TEXT("Observable %s actor does not have an observable component"),
						*GetNameSafe(ObservedCondition.ObservableActor));
			}
		}
	} 
}

void UObserverComponent::CheckObservableStates()
{
	bool bAllActive = true;

	for (TPair<UObservableComponent*, UObserverCondition*> ObservableCondition : ObservableToCondition)
	{
		UObservableComponent* ObservableComponent = ObservableCondition.Key;
		UObserverCondition* ObserverCondition = ObservableCondition.Value;
		
		if (ObservableComponent && ObserverCondition)
		{
			bool bHasConditionMet = ObserverCondition->IsConditionMet_Implementation(ObservableComponent->GetObservableState());
			if(!bHasConditionMet)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s hasn't met condition"), *GetNameSafe(ObservableComponent->GetOwner()));
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
	
	if(bConditionsMet == bAllActive)
	{
		return;
	}
	
	if(!bCanTriggerAgain && bHasTriggered)
	{
		UE_LOG(LogTemp, Warning, TEXT("Observer has already triggered. Can't trigger again"));
		return;
	}
	
	bConditionsMet = bAllActive;
	OnAllObservablesActivated.Broadcast(bConditionsMet);
	UE_LOG(LogTemp, Warning, TEXT("Observable states have changed"));
	
	if (bAllActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("All observer conditions have met"));
		bHasTriggered = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some observer conditions haven't met"));
	}
}

void UObserverComponent::OnObservableStateChanged(UObservableComponent* InChangedObservable)
{
	CheckObservableStates();
}

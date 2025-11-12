// Fill out your copyright notice in the Description page of Project Settings.


#include "ObserverCondition.h"

#include "CoreTypes/ObservingTypes/ObservableDataTypes/ObservableDataTypes.h"
#include "CoreTypes/ObservingTypes/ObservableStates/ObservableState.h"

DEFINE_LOG_CATEGORY(LogFpObserverCondition);

bool UObserverCondition::IsConditionMet_Implementation(UObservableState* InObservableState) const
{
	return true;
}

bool UBoolObserverCondition::IsConditionMet_Implementation(UObservableState* InObservableState) const
{
	if(InObservableState)
	{
		const FInstancedStruct& StateStruct = InObservableState->GetStateValue();
		if(!StateStruct.IsValid())
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("State instanced struct is invalid"))
			return false;
		}
		
		const FBoolObservableData* BoolObservableData = StateStruct.GetPtr<FBoolObservableData>();
		if(!BoolObservableData)
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("Failed to get FBoolObservableData from the state struct"))
			return false;
		}
		
		return bExpectedValue == BoolObservableData->bValue;
	}
	
	return false;
}

bool UTriggerObserverCondition::IsConditionMet_Implementation(UObservableState* InObservableState) const
{
	if(InObservableState)
	{
		const FInstancedStruct& StateStruct = InObservableState->GetStateValue();
		if(!StateStruct.IsValid())
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("State instanced struct is invalid"))
			return false;
		}
		
		const FTriggerObservableData* TriggerObservableData = StateStruct.GetPtr<FTriggerObservableData>();
		if(!TriggerObservableData)
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("Failed to get FTriggerObservableData from the state struct"))
			return false;
		}
		
		return TriggerObservableData->bHasTriggered;
	}
	
	return false;
}

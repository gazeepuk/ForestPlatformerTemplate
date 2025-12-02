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

bool UFloatObserverCondition::IsConditionMet_Implementation(UObservableState* InObservableState) const
{
	if(InObservableState)
	{
		const FInstancedStruct& StateStruct = InObservableState->GetStateValue();
		if(!StateStruct.IsValid())
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("State instanced struct is invalid"))
			return false;
		}
		
		const FFloatObservableData* FloatObservableData = StateStruct.GetPtr<FFloatObservableData>();
		if(!FloatObservableData)
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("Failed to get FFloatObservableData from the state struct"))
			return false;
		}
		
		switch (ComparisonType)
		{
		case EFPComparisonType::LessThan:
			return FloatObservableData->FloatValue < ExpectedValue;
		case EFPComparisonType::EqualsTo:
			return FloatObservableData->FloatValue == ExpectedValue;
		case EFPComparisonType::GreaterThan:
			return FloatObservableData->FloatValue > ExpectedValue;
		case EFPComparisonType::NotEqual:
			return FloatObservableData->FloatValue != ExpectedValue;
		}
	}
	
	return false;
}

bool UInt32ObserverCondition::IsConditionMet_Implementation(UObservableState* InObservableState) const
{
	if(InObservableState)
	{
		const FInstancedStruct& StateStruct = InObservableState->GetStateValue();
		if(!StateStruct.IsValid())
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("State instanced struct is invalid"))
			return false;
		}
		
		const FInt32ObservableData* Int32ObservableData = StateStruct.GetPtr<FInt32ObservableData>();
		if(!Int32ObservableData)
		{
			UE_LOG(LogFpObserverCondition, Warning, TEXT("Failed to get FInt32ObservableData from the state struct"))
			return false;
		}

		switch (ComparisonType)
		{
		case EFPComparisonType::LessThan:
			return Int32ObservableData->IntValue < ExpectedValue;
		case EFPComparisonType::EqualsTo:
			return Int32ObservableData->IntValue == ExpectedValue;
		case EFPComparisonType::GreaterThan:
			return Int32ObservableData->IntValue > ExpectedValue;
		case EFPComparisonType::NotEqual:
			return Int32ObservableData->IntValue != ExpectedValue;
		}
	}
	
	return false;
}

bool UVectorObserverCondition::IsConditionMet_Implementation(UObservableState* InObservableState) const
{
	if(InObservableState)
	{
		const FInstancedStruct& StateStruct = InObservableState->GetStateValue();
		if(!StateStruct.IsValid())
		{
			UE_LOG(LogFpObserverCondition, Error, TEXT("State instanced struct is invalid"))
			return false;
		}
		
		const FVectorObservableData* VectorObservableData = StateStruct.GetPtr<FVectorObservableData>();
		if(!VectorObservableData)
		{
			UE_LOG(LogFpObserverCondition, Error, TEXT("Failed to get FVectorObservableData from the state struct"))
			return false;
		}
		
		return VectorObservableData->VectorValue == ExpectedValue;
	}
	
	return false;
}

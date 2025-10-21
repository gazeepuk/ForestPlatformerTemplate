// Fill out your copyright notice in the Description page of Project Settings.


#include "ObservableState.h"

#include "CoreTypes/ObservingTypes/ObservableDataTypes/ObservableDataTypes.h"

FInstancedStruct UObservableState::GetStateValue_Implementation()
{
	return FInstancedStruct();
}

void UObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
}

#pragma region BoolObservableState

FInstancedStruct UBoolObservableState::GetStateValue_Implementation()
{
	return FInstancedStruct::Make(FBoolObservableData(bValue));
}

void UBoolObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	if(const FBoolObservableData* BoolObservableData = NewValue.GetPtr<FBoolObservableData>())
	{
		if(bValue != BoolObservableData->bValue)
		{
			bValue = BoolObservableData->bValue;
			OnStateChanged.Broadcast();
		}
	}
}

bool UBoolObservableState::GetStateBoolValue()
{
	FInstancedStruct StateStruct = GetStateValue();
	if(!StateStruct.IsValid())
	{
		return false;
	}
	
	if(const FBoolObservableData* BoolObservableData = StateStruct.GetPtr<FBoolObservableData>())
	{
		return BoolObservableData->bValue;
	}

	return false;
}

void UBoolObservableState::SetStateBoolValue(bool bNewValue)
{
	SetStateValue(FInstancedStruct::Make(FBoolObservableData(bNewValue)));
}

#pragma endregion


#pragma region ToggleObservableState

void UToggleObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	UBoolObservableState::SetStateValue_Implementation(FInstancedStruct::Make(FBoolObservableData(!GetStateBoolValue())));
}

void UToggleObservableState::ToggleState()
{
	SetStateValue_Implementation(FInstancedStruct());
}

#pragma endregion


#pragma region TriggerObservableState

FInstancedStruct UTriggerObservableState::GetStateValue_Implementation()
{
	return FInstancedStruct::Make(FTriggerObservableData(bHasTriggered));
}

void UTriggerObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	if(!bCanTriggerAgain && bHasTriggered)
	{
		return;
	}
	
	bHasTriggered = true;
	OnStateChanged.Broadcast();
}

void UTriggerObservableState::ResetTrigger()
{
	bHasTriggered = false;
}

void UTriggerObservableState::TriggerState()
{
	SetStateValue(FInstancedStruct::Make(FTriggerObservableData(true)));
}

#pragma endregion
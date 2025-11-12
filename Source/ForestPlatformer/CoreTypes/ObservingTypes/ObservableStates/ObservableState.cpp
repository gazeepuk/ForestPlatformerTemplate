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
	// Casts NewValue to bool struct and sets its value
	if(const FBoolObservableData* BoolObservableData = NewValue.GetPtr<FBoolObservableData>())
	{
		if(bValue != BoolObservableData->bValue)
		{
			bValue = BoolObservableData->bValue;
			OnStateChanged.Broadcast();
		}
	}
}

bool UBoolObservableState::GetStateBoolValue() const
{
	return bValue;
}

void UBoolObservableState::SetStateBoolValue(bool bNewValue)
{
	SetStateValue(FInstancedStruct::Make(FBoolObservableData(bNewValue)));
}

#pragma endregion


#pragma region ToggleObservableState

void UToggleObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	// Sets the opposite of the current bool value
	UBoolObservableState::SetStateValue_Implementation(FInstancedStruct::Make(FBoolObservableData(!GetStateBoolValue())));
}

void UToggleObservableState::ToggleState()
{
	// Uses empty struct, because the input value isn't used in setter function
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
	// Prevents re-triggering
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
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

#pragma region FloatObservableState
void UFloatObservableState::SetStateFloatValue(float InNewValue)
{
	SetStateValue(FInstancedStruct::Make(FFloatObservableData(InNewValue)));
}

float UFloatObservableState::GetStateFloatValue() const
{
	return Value;
}

FInstancedStruct UFloatObservableState::GetStateValue_Implementation()
{
	return FInstancedStruct::Make(FFloatObservableData(Value));
}

void UFloatObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	if(const FFloatObservableData* FloatData = NewValue.GetPtr<FFloatObservableData>())
	{
		if(Value != FloatData->FloatValue)
		{
			Value = FloatData->FloatValue;
			OnStateChanged.Broadcast();
		}
	}
}
#pragma endregion

#pragma region Int32ObservableState
void UInt32ObservableState::SetStateInt32Value(int32 InNewValue)
{
	SetStateValue(FInstancedStruct::Make(FInt32ObservableData(InNewValue)));
}

int32 UInt32ObservableState::GetStateInt32Value() const
{
	return Value;
}

FInstancedStruct UInt32ObservableState::GetStateValue_Implementation()
{
	return FInstancedStruct::Make(FInt32ObservableData(Value));
}

void UInt32ObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	if(const FInt32ObservableData* IntData = NewValue.GetPtr<FInt32ObservableData>())
	{
		if(Value != IntData->IntValue)
		{
			Value = IntData->IntValue;
			OnStateChanged.Broadcast();
		}
	}
}
#pragma endregion

#pragma region VectorObservableState
void UVectorObservableState::SetStateVectorValue(FVector InNewValue)
{
	SetStateValue(FInstancedStruct::Make(FVectorObservableData(InNewValue)));
}

FVector UVectorObservableState::GetStateVectorValue() const
{
	return Value;
}

FInstancedStruct UVectorObservableState::GetStateValue_Implementation()
{
	return FInstancedStruct::Make(FVectorObservableData(Value));
}

void UVectorObservableState::SetStateValue_Implementation(const FInstancedStruct& NewValue)
{
	if(const FVectorObservableData* VectorData = NewValue.GetPtr<FVectorObservableData>())
	{
		if(Value != VectorData->VectorValue)
		{
			Value = VectorData->VectorValue;
			OnStateChanged.Broadcast();
		}
	}
}
#pragma endregion
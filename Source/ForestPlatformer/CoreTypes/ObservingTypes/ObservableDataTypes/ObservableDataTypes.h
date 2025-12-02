#pragma once

#include "CoreMinimal.h"
#include "ObservableDataTypes.generated.h"

/**
 * Wrapping structs used for observable states to change their values	 
 */


USTRUCT(BlueprintType)
struct FBoolObservableData
{
	GENERATED_BODY()

	FBoolObservableData() : bValue(false)
	{
		
	}

	FBoolObservableData(const bool bNewValue)
	{
		bValue = bNewValue;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bValue;
};

USTRUCT(BlueprintType)
struct FTriggerObservableData
{
	GENERATED_BODY()

	FTriggerObservableData() : bHasTriggered(false)
	{
		
	}

	FTriggerObservableData(const bool bNewHasTriggered)
	{
		bHasTriggered = bNewHasTriggered;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasTriggered;
};

USTRUCT(BlueprintType)
struct FVectorObservableData
{
	GENERATED_BODY()
	
	FVectorObservableData() : VectorValue(FVector::ZeroVector)
	{
     
	}

	FVectorObservableData(const FVector& InVectorValue)
	{
		VectorValue= InVectorValue;
	}
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector VectorValue;
};

USTRUCT(BlueprintType)
struct FInt32ObservableData
{
	GENERATED_BODY()
	
	FInt32ObservableData() : IntValue(0)
	{
     
	}

	FInt32ObservableData(const int32 InIntValue)
	{
		IntValue= InIntValue;
	}
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IntValue;
};

USTRUCT(BlueprintType)
struct FFloatObservableData
{
	GENERATED_BODY()
	
	FFloatObservableData() : FloatValue(0.f)
	{
     
	}

	FFloatObservableData(const float InFloatValue)
	{
		FloatValue= InFloatValue;
	}
  
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FloatValue;
};
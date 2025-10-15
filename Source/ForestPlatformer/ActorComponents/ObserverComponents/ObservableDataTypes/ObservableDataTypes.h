#pragma once

#include "CoreMinimal.h"
#include "ObservableDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FBoolObservableData
{
	GENERATED_BODY()

	FBoolObservableData() : bValue(false)
	{
		
	}

	FBoolObservableData(const bool bBoolValue)
	{
		bValue = bBoolValue;
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
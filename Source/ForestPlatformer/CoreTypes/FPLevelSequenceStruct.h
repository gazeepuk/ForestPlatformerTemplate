#pragma once

#include "Engine/EngineTypes.h"
#include "FPLevelSequenceStruct.generated.h"

/** Contains data for DataTable that stores level sequences for level transition */
USTRUCT(BlueprintType)
struct FFPLevelSequence : public FTableRowBase
{
	GENERATED_BODY()
	
	FORCEINLINE bool IsValid() const
	{
		return !CurrentLevel.IsNull() && !NextLevel.IsNull();
	}
	
	/** Current Level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> CurrentLevel;

	/** Reference to a level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> NextLevel;
};

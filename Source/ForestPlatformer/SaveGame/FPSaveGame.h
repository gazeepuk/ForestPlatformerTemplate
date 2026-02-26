// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/SavableData/FPSavableDataTypes.h"
#include "GameFramework/SaveGame.h"
#include "FPSaveGame.generated.h"

/**
 * 
 */

UCLASS()
class FORESTPLATFORMER_API UFPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/* Stores data for all levels */
	UPROPERTY()
	TMap<FName, FFPLevelData> LevelDataMap;

	/* Stores the last opened level name */
	UPROPERTY()
	FName CurrentLevelName;

	/* Stores global player data */
	UPROPERTY()
	FFPPlayerProgressData PlayerProgressData;
};

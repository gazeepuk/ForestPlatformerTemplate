// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/FPSavableDataTypes.h"
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
	TMap<FString, FFPLevelData> LevelDataMap;

	/* Stores the last opened level name */
	UPROPERTY()
	FString CurrentLevelName;

	/* Stores global player data */
	UPROPERTY()
	FFPPlayerProgressData PlayerProgressData;
};

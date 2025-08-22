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
	UPROPERTY()
	TMap<FString, FFPLevelData> LevelDataMap;

	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FFPPlayerProgressData PlayerProgressData;
};

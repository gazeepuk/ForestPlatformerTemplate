#pragma once

#include "Engine/EngineTypes.h"
#include "FPSavableDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FFPSavableData
{
	GENERATED_BODY()

	FFPSavableData() : ObjectType(NAME_None), bActive(true), DataType(NAME_None) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DataType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<uint8> CustomData;
};

USTRUCT()
struct FFPPlayerProgressData
{
	GENERATED_BODY()

	FFPPlayerProgressData() : MaxHealth(3.f), CoinsValue(0) {}

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	int32 CoinsValue;
};

USTRUCT(BlueprintType)
struct FFPLevelData
{
	GENERATED_BODY()

	FFPLevelData() : LastActiveCheckpointID(NAME_None), LastCheckpointSpawnPoint(FTransform::Identity), PlayersCurrentHealth(0.f), bLevelCompleted(false){}

	bool TrySaveActor(const AActor* InActor);
	
	UPROPERTY()
	FName LastActiveCheckpointID;

	UPROPERTY()
	FTransform LastCheckpointSpawnPoint;
	
	UPROPERTY()
	TMap<FName, FFPSavableData> SavedObjects;

	UPROPERTY()
	float PlayersCurrentHealth;
	
	UPROPERTY()
	bool bLevelCompleted;
};
#pragma once

#include "Engine/EngineTypes.h"
#include "FPSavableDataTypes.generated.h"

/** Contains save data for savable actors */
USTRUCT(BlueprintType)
struct FFPSavableData
{
	GENERATED_BODY()

	FFPSavableData() : ObjectType(NAME_None), bActive(true) {}

	/** Type identifier for the objects being saved (Pickup, Checkpoint) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectType;

	/** Indicates whether the object is active in the game world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive; 

	/** Raw byte data containing the serialized state of the object */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<uint8> CustomData;
};

/** Contains persistent player progression data carries between levels */
USTRUCT()
struct FFPPlayerProgressData
{
	GENERATED_BODY()

	FFPPlayerProgressData() : MaxHealth(3.f), CoinsValue(0) {}

	/** The player's max health value */
	UPROPERTY()
	float MaxHealth;

	/** The total number of coins collected by the player */
	UPROPERTY()
	int32 CoinsValue;
};

/** Contains all save data for specific game level */
USTRUCT(BlueprintType)
struct FFPLevelData
{
	GENERATED_BODY()

	FFPLevelData() : LastActiveCheckpointID(NAME_None), LastCheckpointSpawnPoint(FTransform::Identity), PlayersCurrentHealth(0.f), bLevelCompleted(false){}

	/**
	 * Attempts to save the state of the provided actor to this level data
	 * @param InActor - The actor to save
	 * @return True if the actor was successfully saved, false otherwise 
	 */
	bool TrySaveActor(const AActor* InActor);	

	/** Identifier of the last activated checkpoint ID in this level */
	UPROPERTY()
	FName LastActiveCheckpointID;

	/** Transform of the last activated checkpoint spawn point in the level */
	UPROPERTY()
	FTransform LastCheckpointSpawnPoint;

	/** Map of all objects in the level, keyed by their unique identifier SaveID*/
	UPROPERTY()
	TMap<FName, FFPSavableData> SavedObjects;

	/** The player's current health value when this level was saved */
	UPROPERTY()
	float PlayersCurrentHealth;

	/** Indicates whether the level has been completed */
	UPROPERTY()
	bool bLevelCompleted;
};
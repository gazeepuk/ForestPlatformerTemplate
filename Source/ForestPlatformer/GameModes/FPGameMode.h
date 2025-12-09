// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelCompleted);

class ISavableActorInterface;
class AFPCheckpoint;
class UFPSaveGame;

/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API AFPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(APlayerController* InPlayerController);

	/* Register a checkpoint and save data */
	UFUNCTION(BlueprintCallable)
	void RegisterCheckpoint(AFPCheckpoint* InCheckpoint);

	/* Load game data from save */ 
	UFUNCTION(BlueprintCallable)
	void InitGameFromSave();

	/* Return LastCheckpointID */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetLastCheckpointID() const { return LastCheckpointID; }

	/* Return last activated checkpoint's spawn point transform*/
	UFUNCTION(BlueprintPure)
	FTransform GetLastCheckpointSpawnPoint() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandlePlayerDeath(APlayerController* InPlayerController, APawn* InPlayerPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CompleteLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TransitToNextLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	TSoftObjectPtr<UWorld> GetNextLevel();
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelCompleted OnLevelCompleted;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDisableInputOnLevelComplete = true;

protected:
	virtual void BeginPlay() override;
	// Find a checkpoint by ID
	AFPCheckpoint* FindCheckpointByID(const FName& InCheckpointID) const;
	
	UPROPERTY()
	TMap<FName, AFPCheckpoint*> CheckpointsMap;
	
	UPROPERTY(BlueprintReadOnly)
	FName LastCheckpointID;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCheckpoint> LastActiveCheckpoint;

	UPROPERTY(BlueprintReadOnly)
	FTransform LastCheckpointSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> LevelSequence;
};

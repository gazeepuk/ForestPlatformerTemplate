// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/FPSaveGame.h"
#include "Subsystems/SaveGameSubsystem.h"
#include "FPGameMode.generated.h"

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

	UFUNCTION(BlueprintCallable)
	void RegisterCheckpoint(AFPCheckpoint* InCheckpoint);

	UFUNCTION(BlueprintCallable)
	void InitGameFromSave();

	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetLastCheckpointID() const { return LastCheckpointID; }

	UFUNCTION(BlueprintPure)
	FTransform GetLastCheckpointSpawnPoint() const;
	
protected:
	virtual void BeginPlay() override;
	
	AFPCheckpoint* FindCheckpointByID(const FName& InCheckpointID) const;
	
	UPROPERTY()
	TMap<FName, AFPCheckpoint*> CheckpointsMap;
	
	UPROPERTY(BlueprintReadOnly)
	FName LastCheckpointID;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCheckpoint> LastActiveCheckpoint;

	UPROPERTY(BlueprintReadOnly)
	FTransform LastCheckpointSpawnPoint;
	
	TMap<FName, FFPSavableData> PendingSavableData;
};

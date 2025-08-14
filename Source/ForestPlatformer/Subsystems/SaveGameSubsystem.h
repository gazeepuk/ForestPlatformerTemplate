// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/FPSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSaveGameLoaded, const FString&, LoadedSlotName, const int32, LoadedUserIndex, USaveGame*, LoadedSaveData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSaveGameSaved, const FString&, SavedSlotName, const int32, SavedUserIndex, bool, bSavedSuccessfully);

class UFPSaveGame;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintPure)
	FString GetCleanLevelName() const;
	
	UFUNCTION(BlueprintCallable)
	void LoadGameSlot(const FString& SlotName = "SaveSlot", const int32 UserIndex = 0, const bool bAsyncLoad = false);
	void WriteSaveData();
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot(const FString& SlotName = "SaveSlot", const int32 UserIndex = 0, const bool bAsyncSave = false);
	UFUNCTION(BlueprintPure)
	FORCEINLINE UFPSaveGame* GetSaveGame() const { return SaveGame; }

	UFUNCTION(BlueprintCallable)
	void LoadCurrentLevelFromSave();
	
	FFPLevelData* GetCurrentLevelData() const;
	UFUNCTION(BlueprintCallable)
	void AddPendingSavableObjects(const AActor* InSavableActor);

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameLoaded OnSaveGameLoaded;
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSaved OnSaveGameSaved;

protected:
	UPROPERTY()
	TObjectPtr<UFPSaveGame> SaveGame;

	UPROPERTY()
	TMap<FName, FFPSavableData> PendingSavableData;
};

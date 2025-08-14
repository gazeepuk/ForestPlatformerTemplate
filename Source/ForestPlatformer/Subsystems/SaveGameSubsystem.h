// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/FPSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

#define DEFAULT_SAVE_SLOT_NAME "SaveSlot"

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
	FORCEINLINE FString GetCurrentSlotName() const { return CurrentSlotName; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentSlotName(const FString& NewSlotName);
	
	UFUNCTION(BlueprintPure)
	FString GetCleanLevelName() const;
	
	FFPLevelData* GetCurrentLevelData() const;
	
	UFUNCTION(BlueprintPure)
	FName GetCurrentLastCheckpointID() const;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE UFPSaveGame* GetSaveGame() const { return SaveGame; }
	
	UFUNCTION(BlueprintCallable)
	void LoadGameSlot(const FString& SlotName = "SaveSlot", const int32 UserIndex = 0, const bool bAsyncLoad = false);
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot(const FString& SlotName = "SaveSlot", const int32 UserIndex = 0, const bool bAsyncSave = false);

	void WriteSaveData();

	UFUNCTION(BlueprintCallable)
	void AddPendingSavableObjects(const AActor* InSavableActor);

	UFUNCTION(BlueprintCallable)
	void LoadCurrentLevelFromSave() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameLoaded OnSaveGameLoaded;
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSaved OnSaveGameSaved;

protected:
	UPROPERTY()
	TObjectPtr<UFPSaveGame> SaveGame;

	UPROPERTY()
	TMap<FName, FFPSavableData> PendingSavableData;

	UPROPERTY(BlueprintReadOnly)
	FString CurrentSlotName;
};

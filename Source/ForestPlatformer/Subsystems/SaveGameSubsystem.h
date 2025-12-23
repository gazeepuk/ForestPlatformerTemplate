// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/FPSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpSaveSubsystem, Log, All);

#define DEFAULT_SAVE_SLOT_NAME TEXT("SaveSlot")
#define DEFAULT_LEVEL_REGISTRY_TABLE_PATH TEXT("/Game/Miscellaneous/LevelRegistry/DT_LevelRegistry.DT_LevelRegistry")

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

	/** Returns the name of the current active save slot */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetCurrentSlotName() const { return CurrentSlotName; }

	/** Sets the name of the current save slot */
	UFUNCTION(BlueprintCallable)
	void SetCurrentSlotName(const FString& NewSlotName);

	/** Sets a new path for level registry data table */
	UFUNCTION(BlueprintCallable)
	void SetLevelRegistryTablePath(const FString& NewPath);
	
	/** Returns the name of the current active save slot */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentUserIndex() const { return CurrentUserIndex; }

	/** Sets the name of the current save slot */
	UFUNCTION(BlueprintCallable)
	void SetCurrentUserIndex(int32 NewUserIndex);
	
	/** Returns level registry level table */
	UFUNCTION(BlueprintPure)
	UDataTable* GetLevelRegistryTable();
	
	/** Returns the clean name of the current level */
	UFUNCTION(BlueprintPure)
	FName GetLevelID(const FString& InLevelName);
	
	/** Returns the save data for the current level data from loaded save game */
	FFPLevelData* GetCurrentLevelData();

	/** Returns the save data for the specified level */
	FFPLevelData* GetLevelData(const FString& InLevelName);
	
	/** Returns the ID of the last activated checkpoint in the current level */
	UFUNCTION(BlueprintPure)
	FName GetCurrentLastCheckpointID();

	/** Returns the spawn point of the last activated checkpoint in the current level */
	UFUNCTION(BlueprintPure)
	FTransform GetCurrentLastCheckpointSpawnPoint();

	/** Returns the currently loaded save game */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UFPSaveGame* GetSaveGame() const { return SaveGame; }

	/**
	 * Loads a game save from the specified slot if it exists, or creates a new one 
	 *
	 * @param SlotName		Name of slot
	 * @param UserIndex		User index
	 * @param bAsyncLoad	Either Load asynchronously or synchronously
	 */
	UFUNCTION(BlueprintCallable)
	void LoadGameSlot(const FString& SlotName = "SaveSlot", const int32 UserIndex = 0, const bool bAsyncLoad = false);
	/**
	 * Save a game save to the specified slot
	 *
	 * @param SlotName		Name of slot
	 * @param UserIndex		User index
	 * @param bAsyncSave	Either save asynchronously or synchronously
	 */
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot(const FString& SlotName = "SaveSlot", const int32 UserIndex = 0, const bool bAsyncSave = false);

	UFUNCTION(BlueprintCallable)
	/** Writes the current level's data to the save game */
	void WriteCurrentLevelSaveData();
	
	/*
	 * Adds a savable actor to be processed during the next save.
	 * Useful for actors that may be destroyed before the save occurs saving
	 */
	UFUNCTION(BlueprintCallable)
	void AddPendingSavableActor(const AActor* InSavableActor);

	/** Loads saved data of the current level and applies it to all savable actors */
	UFUNCTION(BlueprintCallable)
	void LoadCurrentLevelFromSave();

	/** Loads player data from save */
	UFUNCTION(BlueprintCallable)
	void LoadPlayerCharacterLevelDataFromSave();

	/** Resets levels data for the specified level */
	UFUNCTION(BlueprintCallable)
	void ResetLevelData(FString InLevelName, bool bHardReset = false);

	/** Marks level completed and save the data */
	UFUNCTION(BlueprintCallable)
	void CompleteLevel(FString InLevelName);
	
	/** Delegate that broadcasts after a save game has been loaded asynchronously */
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameLoaded OnGameLoaded;
	/** Delegate that broadcasts after a save game has been saved asynchronously */
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSaved OnGameSaved;

protected:
	/** Loads level registry DataTable */
	UFUNCTION(BlueprintCallable)
	void LoadLevelRegistryTable();
	
	/** the current loaded game save */
	UPROPERTY()
	TObjectPtr<UFPSaveGame> SaveGame;

	/** Temporary storage for savable data that will be written during the next save operation */
	UPROPERTY()
	TMap<FName, FFPSavableData> PendingSavableData;

	/** The name of the current active save slot */
	UPROPERTY(BlueprintReadOnly)
	FString CurrentSlotName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentUserIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	FString LevelRegistryTablePath;
	
private:

	/** Table with level object reference and its ID*/
	UPROPERTY() 
	TObjectPtr<UDataTable> LevelRegistryTable;

	UFUNCTION()
	void HandleAsyncLoadGame(const FString& LoadedSlotName, const int32 LoadedUserIndex, USaveGame* LoadedSaveGame);
	
	UFUNCTION()
	void HandleAsyncSaveGame(const FString& SavedSlotName, const int32 SavedUserIndex, bool bSavedSuccessfully);
};

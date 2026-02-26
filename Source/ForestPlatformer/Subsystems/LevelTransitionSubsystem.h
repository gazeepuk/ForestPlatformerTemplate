// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelTransitionSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpLevelTransition, Log, All)

#define DEFAULT_LEVEL_SEQUENCES_TABLE_PATH TEXT("/Game/ForestPlatformer/Miscellaneous/LevelSequences/DT_LevelSequences.DT_LevelSequences")

/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API ULevelTransitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Returns current path to a data table with the level sequences*/
	UFUNCTION(BlueprintPure)
	FString GetLevelSequencesTablePath() const;

	/** Sets new path to a ata table with the level sequences */
	UFUNCTION(BlueprintCallable)
	void SetLevelSequencesTablePath(const FString& InNewPath);

	/** Loads a data table with LevelSequencesTablePath */
	UFUNCTION(BlueprintCallable)
	void LoadLevelSequencesDataTable();

	/** Returns next level after the current level */
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetSoftNextLevelObject();
protected:
	/** A path to a data table containing the level sequences */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString LevelSequencesTablePath;

	/** Cached loaded data table with the level sequences */
	UPROPERTY()
	TObjectPtr<UDataTable> LevelSequencesDataTable;
};

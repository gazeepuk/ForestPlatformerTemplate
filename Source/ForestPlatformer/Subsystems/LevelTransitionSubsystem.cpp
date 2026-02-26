// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionSubsystem.h"

#include "CoreTypes/LevelSequenceStruct/FPLevelSequenceStruct.h"

DEFINE_LOG_CATEGORY(LogFpLevelTransition)

void ULevelTransitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(LevelSequencesTablePath.IsEmpty())
	{
		LevelSequencesTablePath = DEFAULT_LEVEL_SEQUENCES_TABLE_PATH;
	}
}

FString ULevelTransitionSubsystem::GetLevelSequencesTablePath() const
{
	return LevelSequencesTablePath;
}

void ULevelTransitionSubsystem::SetLevelSequencesTablePath(const FString& InNewPath)
{
	LevelSequencesTablePath = InNewPath.IsEmpty() ? DEFAULT_LEVEL_SEQUENCES_TABLE_PATH : InNewPath;
}

void ULevelTransitionSubsystem::LoadLevelSequencesDataTable()
{
	LevelSequencesDataTable = LoadObject<UDataTable>(nullptr, *LevelSequencesTablePath);
	if(!LevelSequencesDataTable)
	{
		UE_LOG(LogFpLevelTransition, Error, TEXT("Failed loading Level Registry Table"));
	}
}

TSoftObjectPtr<UWorld> ULevelTransitionSubsystem::GetSoftNextLevelObject()
{
	if(!LevelSequencesDataTable)
	{
		// Try to load the table, if it's invalid
		LoadLevelSequencesDataTable();
		if(!LevelSequencesDataTable)
		{
			return nullptr;
		}
	}

	if(!GetWorld())
	{
		return nullptr;
	}

	FString ContextString("LevelSequencesLookup");
	TArray<FName> LevelSequencesRowNames = LevelSequencesDataTable->GetRowNames();

	for (const FName& LevelSequencesRowName : LevelSequencesRowNames)
	{
		if(FFPLevelSequence* Row = LevelSequencesDataTable->FindRow<FFPLevelSequence>(LevelSequencesRowName, ContextString))
		{
			if(!Row->IsValid())
			{
				continue;
			}

			FString RowLevelName = Row->CurrentLevel.GetAssetName();
			if(RowLevelName == GetWorld()->GetName())
			{
				return Row->NextLevel;
			}
		}
	}

	return nullptr;
}
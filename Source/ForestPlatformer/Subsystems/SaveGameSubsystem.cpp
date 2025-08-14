// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubsystem.h"

#include "GameModes/FPGameMode.h"
#include "Interfaces/SavableObjectInterface.h"
#include "SaveGame/FPSaveGame.h"

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadGameSlot("SaveSlot", 0, false);
}

FString USaveGameSubsystem::GetCleanLevelName() const
{
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	return LevelName;
}

void USaveGameSubsystem::LoadGameSlot(const FString& SlotName, const int32 UserIndex, const bool bAsync)
{
	
	if(UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		// Async loading SaveData
		if(bAsync)
		{
			UGameplayStatics::AsyncLoadGameFromSlot(SlotName, UserIndex,
			FAsyncLoadGameFromSlotDelegate::CreateLambda([&](const FString& LoadedSlotName, const int32 LoadedUserIndex, USaveGame* LoadedSaveData)
			{
				if(IsValid(this))
				{
					SaveGame = Cast<UFPSaveGame>(LoadedSaveData);
					OnSaveGameLoaded.Broadcast(LoadedSlotName, LoadedUserIndex, SaveGame);
				}
			}));
		}
		//Sync loading SaveData
		else
		{
			SaveGame = Cast<UFPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
		}

		if(!SaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load existing save. Creating a new save"));
		}
	}

	if(!SaveGame)
	{
		SaveGame = Cast<UFPSaveGame>(UGameplayStatics::CreateSaveGameObject(UFPSaveGame::StaticClass()));
		if(!SaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create a new save data"));
		}
	}
}

void USaveGameSubsystem::WriteSaveData()
{
	FString LevelName = GetCleanLevelName();

	FFPLevelData& CurrentLevelData = SaveGame->LevelDataMap.FindOrAdd(LevelName);
	
	if(AFPGameMode* FPGameMode = GetWorld()->GetAuthGameMode<AFPGameMode>())
	{
		SaveGame->LevelDataMap[LevelName].LastActiveCheckpointID = FPGameMode->GetLastCheckpointID();
		SaveGame->CurrentLevelName = LevelName;
	}
	
	TArray<AActor*> AllSavableActors;
	UGameplayStatics::GetAllActorsWithInterface(this, USavableObjectInterface::StaticClass(), AllSavableActors);
	for (AActor* SavableActor : AllSavableActors)
	{
		if(CurrentLevelData.TrySaveActor(SavableActor))
		{
			UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *ISavableObjectInterface::Execute_GetSaveID(SavableActor).ToString());
		}
	}
	
	for (TPair<FName, FFPSavableData> Data : PendingSavableData)
	{
		FFPSavableData& ExistingSaveData = CurrentLevelData.SavedObjects.FindOrAdd(Data.Key);
		ExistingSaveData = Data.Value;
		UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *Data.Key.ToString());
	}
	PendingSavableData.Empty();

	UE_LOG(LogTemp, Display, TEXT("Successfly saved game"));
}

void USaveGameSubsystem::SaveGameSlot(const FString& SlotName, const int32 UserIndex, const bool bAsyncSave)
{
	if(!SaveGame || !GetWorld())
	{
		return;
	}

	WriteSaveData();
	
	if(bAsyncSave)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SaveGame, SlotName, UserIndex,
		FAsyncSaveGameToSlotDelegate::CreateLambda([&](const FString& SavedSlotName, const int32 SavedUserIndex, bool bSavedSuccessfully)
		{
			if(IsValid(this))
			{
				OnSaveGameSaved.Broadcast(SavedSlotName, SavedUserIndex, bSavedSuccessfully);
			}
		}));
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
	}

}

void USaveGameSubsystem::LoadCurrentLevelFromSave()
{
	if(!SaveGame)
	{
		return;
	}
	
	FString LevelName = GetCleanLevelName();

	if(FFPLevelData* LevelData = GetCurrentLevelData())
	{
		TArray<AActor*> AllSavableActors;
		UGameplayStatics::GetAllActorsWithInterface(this, USavableObjectInterface::StaticClass(), AllSavableActors);

		for (AActor* SavableActor : AllSavableActors)
		{
			const FName SaveID = ISavableObjectInterface::Execute_GetSaveID(SavableActor);
			if(SaveID != NAME_None && LevelData->SavedObjects.Contains(SaveID))
			{
				ISavableObjectInterface::Execute_LoadFromSaveData(SavableActor, LevelData->SavedObjects[ISavableObjectInterface::Execute_GetSaveID(SavableActor)]);
			}
		}
	}
}

FFPLevelData* USaveGameSubsystem::GetCurrentLevelData() const
{
	const FString LevelName = GetCleanLevelName();
	return SaveGame->LevelDataMap.Find(LevelName);
}

void USaveGameSubsystem::AddPendingSavableObjects(const AActor* InSavableActor)
{
	if(!InSavableActor || !InSavableActor->Implements<USavableObjectInterface>())
	{
		return;
	}

	FName SaveID = ISavableObjectInterface::Execute_GetSaveID(InSavableActor);
	if(SaveID.IsNone())
	{
		return;
	}
	
	FFPSavableData DataToSave = ISavableObjectInterface::Execute_GetSaveData(InSavableActor);
	PendingSavableData.FindOrAdd(SaveID) = DataToSave;
}
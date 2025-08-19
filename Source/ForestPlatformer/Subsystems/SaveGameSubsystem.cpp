// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubsystem.h"

#include "ActorComponents/HealthComponent/HealthComponent.h"
#include "GameModes/FPGameMode.h"
#include "Interfaces/CoinsWalletInterface.h"
#include "Interfaces/SavableActorInterface.h"
#include "SaveGame/FPSaveGame.h"

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(CurrentSlotName.IsEmpty())
	{
		CurrentSlotName = DEFAULT_SAVE_SLOT_NAME;
	}
	LoadGameSlot(CurrentSlotName, 0, false);
}

void USaveGameSubsystem::SetCurrentSlotName(const FString& NewSlotName)
{
	CurrentSlotName = NewSlotName.IsEmpty() ? DEFAULT_SAVE_SLOT_NAME : NewSlotName;
}

FString USaveGameSubsystem::GetCleanLevelName() const
{
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	return LevelName;
}

FFPLevelData* USaveGameSubsystem::GetCurrentLevelData() const
{
	const FString LevelName = GetCleanLevelName();
	return SaveGame->LevelDataMap.Find(LevelName);
}

FName USaveGameSubsystem::GetCurrentLastCheckpointID() const
{
	if(const FFPLevelData* CurrentLevelData = GetCurrentLevelData())
	{
		return CurrentLevelData->LastActiveCheckpointID;
	}
	return FName();
}

FTransform USaveGameSubsystem::GetCurrentLastCheckpointSpawnPoint() const
{
	if(const FFPLevelData* CurrentLevelData = GetCurrentLevelData())
	{
		return CurrentLevelData->LastCheckpointSpawnPoint;
	}
	return FTransform::Identity;
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
			if(IsValid(this) && bSavedSuccessfully)
			{
				UE_LOG(LogTemp, Display, TEXT("Successfly saved game"));
				OnSaveGameSaved.Broadcast(SavedSlotName, SavedUserIndex, bSavedSuccessfully);
			}
		}));
	}
	else
	{
		if(UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex))
		{
			UE_LOG(LogTemp, Display, TEXT("Successfly saved game"));
		}
	}

}

void USaveGameSubsystem::WriteSaveData()
{
	FString LevelName = GetCleanLevelName();

	FFPLevelData& CurrentLevelData = SaveGame->LevelDataMap.FindOrAdd(LevelName);
	
	if(const AFPGameMode* FPGameMode = GetWorld()->GetAuthGameMode<AFPGameMode>())
	{
		SaveGame->LevelDataMap[LevelName].LastActiveCheckpointID = FPGameMode->GetLastCheckpointID();
		SaveGame->LevelDataMap[LevelName].LastCheckpointSpawnPoint = FPGameMode->GetLastCheckpointSpawnPoint();
		SaveGame->CurrentLevelName = LevelName;
	}
	
	TArray<AActor*> AllSavableActors;
	UGameplayStatics::GetAllActorsWithInterface(this, USavableActorInterface::StaticClass(), AllSavableActors);
	for (AActor* SavableActor : AllSavableActors)
	{
		if(CurrentLevelData.TrySaveActor(SavableActor))
		{
			UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *ISavableActorInterface::Execute_GetSaveID(SavableActor).ToString());
		}
	}
	
	for (TPair<FName, FFPSavableData> Data : PendingSavableData)
	{
		FFPSavableData& ExistingSaveData = CurrentLevelData.SavedObjects.FindOrAdd(Data.Key);
		ExistingSaveData = Data.Value;
		UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *Data.Key.ToString());
	}
	PendingSavableData.Empty();

	// Save Player Data
	if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if(PlayerController->Implements<UCoinsWalletInterface>())
		{
			SaveGame->PlayerProgressData.CoinsValue = ICoinsWalletInterface::Execute_GetCurrentCoins(PlayerController);
		}
		
		if(const APawn* ControlledPawn = PlayerController ? PlayerController->GetPawn() : nullptr)
		{
			if(const UHealthComponent* HealthComponent = ControlledPawn->GetComponentByClass<UHealthComponent>())
			{
				SaveGame->PlayerProgressData.MaxHealth = HealthComponent->GetMaxHealth();
				CurrentLevelData.PlayersCurrentHealth = HealthComponent->GetCurrentHealth();
			}
		}
	}
}

void USaveGameSubsystem::AddPendingSavableObjects(const AActor* InSavableActor)
{
	if(!InSavableActor || !InSavableActor->Implements<USavableActorInterface>())
	{
		return;
	}

	FName SaveID = ISavableActorInterface::Execute_GetSaveID(InSavableActor);
	if(SaveID.IsNone())
	{
		return;
	}
	
	FFPSavableData DataToSave = ISavableActorInterface::Execute_GetSaveData(InSavableActor);
	PendingSavableData.FindOrAdd(SaveID) = DataToSave;
}

void USaveGameSubsystem::LoadCurrentLevelFromSave() const
{
	if(!SaveGame)
	{
		return;
	}
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController)
	{
		if(PlayerController->Implements<UCoinsWalletInterface>())
		{
			ICoinsWalletInterface::Execute_SetCurrentCoins(PlayerController, SaveGame->PlayerProgressData.CoinsValue);
		}
	}
	
	FString LevelName = GetCleanLevelName();

	if(FFPLevelData* LevelData = GetCurrentLevelData())
	{
		TArray<AActor*> AllSavableActors;
		UGameplayStatics::GetAllActorsWithInterface(this, USavableActorInterface::StaticClass(), AllSavableActors);

		for (AActor* SavableActor : AllSavableActors)
		{
			const FName SaveID = ISavableActorInterface::Execute_GetSaveID(SavableActor);
			if(SaveID != NAME_None && LevelData->SavedObjects.Contains(SaveID))
			{
				ISavableActorInterface::Execute_LoadFromSaveData(SavableActor, LevelData->SavedObjects[SaveID]);
			}
		}

		if(const APawn* ControlledPawn = PlayerController ? PlayerController->GetPawn() : nullptr)
		{
			if(UHealthComponent* HealthComponent = ControlledPawn->GetComponentByClass<UHealthComponent>())
			{
				const float PlayerMaxHealth = SaveGame->PlayerProgressData.MaxHealth;
				HealthComponent->SetMaxHealth(PlayerMaxHealth);
				const float PlayerCurrentHealth = LevelData->PlayersCurrentHealth > 0 ? LevelData->PlayersCurrentHealth : PlayerMaxHealth;
				HealthComponent->SetCurrentHealth(PlayerCurrentHealth);
			}
		}
	}
}
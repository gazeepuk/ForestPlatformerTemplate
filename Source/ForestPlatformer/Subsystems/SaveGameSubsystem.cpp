// Developed by Ivan Piankouski. All Rights Reserved


#include "SaveGameSubsystem.h"

#include "ActorComponents/HealthComponent/HealthComponent.h"
#include "ActorComponents/InventoryComponent/InventoryComponent.h"
#include "GameModes/FPGameMode.h"
#include "Interfaces/CoinsWalletInterface.h"
#include "Interfaces/SavableActorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/FPSaveGame.h"

DEFINE_LOG_CATEGORY(LogFpSaveSubsystem);

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(LevelRegistryTablePath.IsEmpty())
	{
		LevelRegistryTablePath = DEFAULT_LEVEL_REGISTRY_TABLE_PATH;
	}
	if(CurrentSlotName.IsEmpty())
	{
		CurrentSlotName = DEFAULT_SAVE_SLOT_NAME;
	}
	LoadGameSlot(CurrentSlotName, CurrentUserIndex, false);
	LoadLevelRegistryTable();
}

void USaveGameSubsystem::SetCurrentSlotName(const FString& NewSlotName)
{
	CurrentSlotName = NewSlotName.IsEmpty() ? DEFAULT_SAVE_SLOT_NAME : NewSlotName;
}

void USaveGameSubsystem::SetLevelRegistryTablePath(const FString& NewPath)
{
	LevelRegistryTablePath = NewPath.IsEmpty() ? DEFAULT_LEVEL_REGISTRY_TABLE_PATH : NewPath;
}

void USaveGameSubsystem::LoadLevelRegistryTable()
{
	LevelRegistryTable = LoadObject<UDataTable>(nullptr, *LevelRegistryTablePath);
	if(!LevelRegistryTable)
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("Failed loading Level Registry Table"));
	}
}

void USaveGameSubsystem::SetCurrentUserIndex(int32 NewUserIndex)
{
	CurrentUserIndex = NewUserIndex;
}

UDataTable* USaveGameSubsystem::GetLevelRegistryTable()
{
	if(LevelRegistryTable)
	{
		return LevelRegistryTable;
	}
	LoadLevelRegistryTable();
	return LevelRegistryTable;
}

FName USaveGameSubsystem::GetLevelID(const FString& InLevelName)
{
	UDataTable* LevelRegistry = GetLevelRegistryTable();
	if(!LevelRegistry)
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("Level Registry Table is invalid. Can't get level ID for the current level"));
		return FName();
	}
	
	FString ContextString("LevelRegistryLookup");
	TArray<FName> LevelRegistryRowNames = LevelRegistry->GetRowNames();

	for (const FName& RowName : LevelRegistryRowNames)
	{
		if(FLevelRegistryRow* Row = LevelRegistry->FindRow<FLevelRegistryRow>(RowName, ContextString))
		{
			if(!Row->IsValid())
			{
				continue;
			}
			
			FString RowLevelName = Row->LevelAsset.GetAssetName();
			if(RowLevelName == InLevelName)
			{
				return Row->LevelID;
			}
		}
	}

	UE_LOG(LogFpSaveSubsystem, Warning, TEXT("LevelId for %s was not found"), *InLevelName);
	return FName();
}

FFPLevelData* USaveGameSubsystem::GetCurrentLevelData()
{
	if(SaveGame)
	{
		const FName LevelName = GetLevelID(GetWorld()->GetName());
		return SaveGame->LevelDataMap.Find(LevelName);
	}
	return nullptr;
}

FFPLevelData* USaveGameSubsystem::GetLevelData(const FString& InLevelName)
{
	const FName LevelName = GetLevelID(InLevelName);
	if(LevelName.IsNone())
	{
		return nullptr;
	}
	
	return &SaveGame->LevelDataMap.FindOrAdd(LevelName);
}

FName USaveGameSubsystem::GetCurrentLastCheckpointID()
{
	if(const FFPLevelData* CurrentLevelData = GetCurrentLevelData())
	{
		return CurrentLevelData->LastActiveCheckpointID;
	}
	return FName();
}

FTransform USaveGameSubsystem::GetCurrentLastCheckpointSpawnPoint()
{
	if(const FFPLevelData* CurrentLevelData = GetCurrentLevelData())
	{
		return CurrentLevelData->LastCheckpointSpawnPoint;
	}
	return FTransform::Identity;
}

void USaveGameSubsystem::LoadGameSlot(const FString& SlotName, const int32 UserIndex, const bool bAsync)
{

	// Load SaveGame if it exists
	if(UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		OnGameStartLoading.Broadcast();
		// Async loading SaveData
		if(bAsync)
		{
			UGameplayStatics::AsyncLoadGameFromSlot(SlotName, UserIndex, FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &ThisClass::HandleAsyncLoadGame));
		}
		//Sync loading SaveData
		else
		{
			SaveGame = Cast<UFPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
			OnGameLoaded.Broadcast(SlotName, UserIndex, SaveGame);
			if(!SaveGame)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to load existing save. Creating a new save"));
			}
		}
	}

	// Create a new SaveGame if the loading failed
	if(!SaveGame)
	{
		OnGameStartLoading.Broadcast();
		SaveGame = Cast<UFPSaveGame>(UGameplayStatics::CreateSaveGameObject(UFPSaveGame::StaticClass()));
		OnGameLoaded.Broadcast(SlotName, UserIndex, SaveGame);

		if(!SaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create a new save data"));
		}
	}
}

void USaveGameSubsystem::HandleAsyncLoadGame(const FString& LoadedSlotName, const int32 LoadedUserIndex,
	USaveGame* LoadedSaveGame)
{
	SaveGame = Cast<UFPSaveGame>(LoadedSaveGame);
	OnGameLoaded.Broadcast(LoadedSlotName, LoadedUserIndex, LoadedSaveGame);
}

void USaveGameSubsystem::SaveGameSlot(const FString& SlotName, const int32 UserIndex, const bool bAsyncSave)

{
	if(!SaveGame)
	{
		// Try to load or create a new save slot, if it does not exist
		LoadGameSlot(SlotName, UserIndex);

		if(!SaveGame)
		{
			UE_LOG(LogTemp, Error, TEXT("SaveGame is invalid. Can't save the data"));
			return;
		}
	}
	
	// Save written data either asynchronously or synchronously
	// Async saving
	if(bAsyncSave)
	{
		OnGameStartSaving.Broadcast();
		UGameplayStatics::AsyncSaveGameToSlot(SaveGame, SlotName, UserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &ThisClass::HandleAsyncSaveGame));
	}
	// Sync saving 
	else
	{
		OnGameStartSaving.Broadcast();
		bool bSavedSuccessfully = UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
		OnGameSaved.Broadcast(SlotName, UserIndex, bSavedSuccessfully);
		
		if(bSavedSuccessfully)
		{
			UE_LOG(LogTemp, Display, TEXT("Successfly saved game"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Failed to save game"));
		}
	}
}


void USaveGameSubsystem::HandleAsyncSaveGame(const FString& SavedSlotName, const int32 SavedUserIndex,
                                             bool bSavedSuccessfully)
{
	if(bSavedSuccessfully)
	{
		UE_LOG(LogTemp, Display, TEXT("Successfly saved game"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to save game"));
	}
	
	OnGameSaved.Broadcast(SavedSlotName, SavedUserIndex, bSavedSuccessfully);
}

void USaveGameSubsystem::WriteCurrentLevelSaveData()
{
	if(!GetWorld())
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("World is invalid. Can't save game"));
		return;
	}

	if(!SaveGame)
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("Save game is invalid during writing save data"));
		return;
	}
	
	// Get current level's name 
	FName LevelName = GetLevelID(GetWorld()->GetName());

	// Find or create a level data
	FFPLevelData& CurrentLevelData = SaveGame->LevelDataMap.FindOrAdd(LevelName);

	// Write checkpoint data and the level name
	if(const AFPGameMode* FPGameMode = GetWorld()->GetAuthGameMode<AFPGameMode>())
	{
		SaveGame->LevelDataMap[LevelName].LastActiveCheckpointID = FPGameMode->GetLastCheckpointID();
		SaveGame->LevelDataMap[LevelName].LastCheckpointSpawnPoint = FPGameMode->GetLastCheckpointSpawnPoint();
		SaveGame->CurrentLevelName = LevelName;
	}

	// Save savable actors
	TArray<AActor*> AllSavableActors;
	UGameplayStatics::GetAllActorsWithInterface(this, USavableActorInterface::StaticClass(), AllSavableActors);
	for (AActor* SavableActor : AllSavableActors)
	{
		if(CurrentLevelData.TrySaveActor(SavableActor))
		{
			UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *ISavableActorInterface::Execute_GetSaveID(SavableActor).ToString());
		}
	}

	// Save the pending data
	for (TPair<FName, FFPSavableData> Data : PendingSavableData)
	{
		FFPSavableData& ExistingSaveData = CurrentLevelData.SavedObjects.FindOrAdd(Data.Key);
		ExistingSaveData = Data.Value;
		UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *Data.Key.ToString());
	}
	// Cleanup the pending data after writing
	PendingSavableData.Empty();

	// Save Player Data
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		// Saves coins
		if(PlayerController->Implements<UCoinsWalletInterface>())
		{
			SaveGame->PlayerProgressData.CoinsValue = ICoinsWalletInterface::Execute_GetCurrentCoins(PlayerController);
		}

		// Saves inventory
		if(const UInventoryComponent* InventoryComponent = PlayerController->GetComponentByClass<UInventoryComponent>())
		{
			SaveGame->PlayerProgressData.PlayerInventory = InventoryComponent->GetInventoryCopy();
		}
		
		// Saves health
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

void USaveGameSubsystem::AddPendingSavableActor(const AActor* InSavableActor)
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

void USaveGameSubsystem::LoadCurrentLevelFromSave()
{
	if(!SaveGame)
	{
		// Tries to load the game slot
		LoadGameSlot(CurrentSlotName);
		if(!SaveGame)
		{
			UE_LOG(LogFpSaveSubsystem, Warning, TEXT("Save Game is invalid. Can't load saved data from %s slot"), *CurrentSlotName);
			return;
		}
	}

	if(!GetWorld())
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("World is invalid. Can't load saved data"))
		return;
	}

	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		// Load player's inventory
		if(UInventoryComponent* PlayerInventoryComponent = PlayerController->GetComponentByClass<UInventoryComponent>())
		{
			PlayerInventoryComponent->LoadInventory(SaveGame->PlayerProgressData.PlayerInventory);
		}
		// Load player's coins
		if(PlayerController->Implements<UCoinsWalletInterface>())
		{
			ICoinsWalletInterface::Execute_SetCurrentCoins(PlayerController, SaveGame->PlayerProgressData.CoinsValue);
		}
	}

	if(FFPLevelData* LevelData = GetCurrentLevelData())
	{
		// Load Savable actors
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
	}
}

void USaveGameSubsystem::LoadPlayerCharacterLevelDataFromSave()
{
	if(!SaveGame)
	{
		// Tries to load the game slot
		LoadGameSlot(CurrentSlotName);
		if(!SaveGame)
		{
			UE_LOG(LogFpSaveSubsystem, Warning, TEXT("Save Game is invalid. Can't load saved data from %s slot"), *CurrentSlotName);
			return;
		}
	}

	if(!GetWorld())
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("World is invalid. Can't load character data"))
		return;
	}
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(!PlayerController)
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("Player Controller is invalid. Can't load character data"))
		return;
	}
	
	const APawn* ControlledPawn = PlayerController ? PlayerController->GetPawn() : nullptr;
	if(!ControlledPawn)
	{
		UE_LOG(LogFpSaveSubsystem, Error, TEXT("Player Character is invalid. Can't load character data"))
		return;
	}
	
	// Loads player's health
	if(UHealthComponent* HealthComponent = ControlledPawn->GetComponentByClass<UHealthComponent>())
	{
		const float PlayerMaxHealth = SaveGame->PlayerProgressData.MaxHealth;

		FFPLevelData* LevelData = GetCurrentLevelData();
		if(!LevelData)
		{
			UE_LOG(LogFpSaveSubsystem, Error, TEXT("Current level save data is invalid. The current level's save data can't be used."))
		}
		
		const float PlayerCurrentHealth = LevelData && LevelData->PlayersCurrentHealth > 0 ? LevelData->PlayersCurrentHealth : PlayerMaxHealth;
		HealthComponent->LoadHealth(PlayerCurrentHealth, PlayerMaxHealth);
	}
}

void USaveGameSubsystem::ResetLevelData(FString InLevelName, bool bHardReset)
{
	if(FFPLevelData* LevelData = GetLevelData(InLevelName))
	{
		// Resets the level completion status 
		LevelData->bLevelCompleted &= !bHardReset;

		// Clears all saved objects
		LevelData->SavedObjects.Empty();

		// Resets player's health
		LevelData->PlayersCurrentHealth = 0;

		// Resets checkpoint data
		LevelData->LastActiveCheckpointID = NAME_None;
		LevelData->LastCheckpointSpawnPoint = FTransform::Identity;

		// Saves the changes
		SaveGameSlot(CurrentSlotName, CurrentUserIndex, false);

		UE_LOG(LogFpSaveSubsystem, Display, TEXT("Restarted %s level."), *InLevelName);
	}
	else
	{
		UE_LOG(LogFpSaveSubsystem, Warning, TEXT("Can't restarted %s level. Level data is invalid"), *InLevelName);
	}
}

void USaveGameSubsystem::CompleteLevel(FString InLevelName)
{
	if(FFPLevelData* LevelData = GetLevelData(InLevelName))
	{
		LevelData->bLevelCompleted = true;

		WriteCurrentLevelSaveData();
		SaveGameSlot(CurrentSlotName, CurrentUserIndex, true);
	}
}

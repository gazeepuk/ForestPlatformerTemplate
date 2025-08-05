// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"

#include "EngineUtils.h"
#include "Checkpoints/FPCheckpoint.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/SavableObjectInterface.h"
#include "SaveGame/FPSaveGame.h"

void AFPGameMode::BeginPlay()
{
	Super::BeginPlay();

	for(TActorIterator<AFPCheckpoint> It(GetWorld()); It; ++It)
	{
		if(*It)
		{
			AllCheckpoints.AddUnique(*It);
		}
	}
	
	LoadGame();
}

void AFPGameMode::RespawnPlayer(APlayerController* InPlayerController)
{
	if(!InPlayerController)
	{
		return;
	}

	if(APawn* PlayerPawn = InPlayerController->GetPawn())
	{
		UClass* PlayerPawnClass = PlayerPawn->GetClass();

		PlayerPawn->Destroy();
		
		FTransform SpawnTransform;
		if(LastCheckpointID != NAME_None)
		{
			AFPCheckpoint** LastCheckpoint = AllCheckpoints.FindByPredicate([this](const AFPCheckpoint* CP)
			{
				return ISavableObjectInterface::Execute_GetSaveID(CP) == LastCheckpointID;
			});

			SpawnTransform = LastCheckpoint ? (*LastCheckpoint)->GetActorTransform() : FTransform::Identity;
		}
		else
		{
			AActor* PlayerStartActor = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
			SpawnTransform = PlayerStartActor ? PlayerStartActor->GetActorTransform() : FTransform::Identity;
		}
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if(APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PlayerPawnClass, SpawnTransform, SpawnParameters))
		{
			InPlayerController->Possess(SpawnedPawn);
		}
	}
}

void AFPGameMode::RegisterCheckpoint(AFPCheckpoint* InCheckpoint)
{
	if(InCheckpoint)
	{
		InCheckpoint->SetCheckpointActivated(true);
		
		LastActiveCheckpoint = InCheckpoint;
		LastCheckpointTransform = InCheckpoint->GetActorTransform();
		LastCheckpointID = InCheckpoint->GetSaveID_Implementation();

		SaveGame();
	}
}

void AFPGameMode::SaveGame()
{
	if(UFPSaveGame* SaveGame = GetOrCreateSaveData<UFPSaveGame>())
	{
		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		FFPLevelData& CurrentLevelData = SaveGame->LevelData.FindOrAdd(LevelName);

		if(LastCheckpointID != NAME_None)
		{
			SaveGame->LevelData[LevelName].LastActiveCheckpointID = LastCheckpointID;
			SaveGame->CurrentLevelName = LevelName;
		}

		for (TPair<FName, FFPSavableData> Data : PendingSavableData)
		{
			FFPSavableData& ExistingSaveData = CurrentLevelData.SavedObjects.FindOrAdd(Data.Key);
			ExistingSaveData = Data.Value;
			UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *Data.Key.ToString());
		}

		PendingSavableData.Empty();
		
		TArray<AActor*> AllSavableActors;
		UGameplayStatics::GetAllActorsWithInterface(this, USavableObjectInterface::StaticClass(), AllSavableActors);

		for (AActor* SavableActor : AllSavableActors)
		{
			if(CurrentLevelData.TrySaveActor(SavableActor))
			{
				UE_LOG(LogTemp, Display, TEXT("Saved Object: %s"), *ISavableObjectInterface::Execute_GetSaveID(SavableActor).ToString());
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGame, "SaveSlot", 0);
		UE_LOG(LogTemp, Display, TEXT("Successfly saved game"));
	}
}

void AFPGameMode::LoadGame()
{
	if(UFPSaveGame* SaveGame = GetOrCreateSaveData<UFPSaveGame>())
	{
		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if(SaveGame->LevelData.Contains(LevelName))
		{
			FFPLevelData* LevelData = SaveGame->LevelData.Find(LevelName);

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

			LastCheckpointID = LevelData->LastActiveCheckpointID;
		}
	}
}

void AFPGameMode::AddPendingSavableObjects(AActor* InSavableActor)
{
	if(InSavableActor && InSavableActor->Implements<USavableObjectInterface>())
	{
		FName SaveID = ISavableObjectInterface::Execute_GetSaveID(InSavableActor);
		FFPSavableData DataToSave = ISavableObjectInterface::Execute_GetSaveData(InSavableActor);

		FFPSavableData& ExistingData = PendingSavableData.FindOrAdd(SaveID);
		ExistingData = DataToSave;
	}
}


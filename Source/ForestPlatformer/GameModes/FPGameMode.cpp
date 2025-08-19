// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"

#include "EngineUtils.h"
#include "Checkpoints/FPCheckpoint.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/SavableActorInterface.h"
#include "Subsystems/SaveGameSubsystem.h"


FTransform AFPGameMode::GetLastCheckpointSpawnPoint() const
{
	const AFPCheckpoint* LastCheckpoint = FindCheckpointByID(LastCheckpointID);
	return LastCheckpoint ? LastCheckpoint->GetSpawnPointTransform() : FTransform::Identity;
}

void AFPGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	for(TActorIterator<AFPCheckpoint> It(GetWorld()); It; ++It)
	{
		if(AFPCheckpoint* Checkpoint = *It)
		{
			const FName CheckpointID = ISavableActorInterface::Execute_GetSaveID(Checkpoint);
			if(!CheckpointID.IsNone())
			{
				CheckpointsMap.Add(CheckpointID, Checkpoint);
			}
		}
	}
	InitGameFromSave();
}

AFPCheckpoint* AFPGameMode::FindCheckpointByID(const FName& InCheckpointID) const
{
	if(InCheckpointID.IsNone())
	{
		return nullptr;
	}

	if(AFPCheckpoint* const* FoundedCheckpoint = CheckpointsMap.Find(InCheckpointID))
	{
		return *FoundedCheckpoint;
	}
	
	return nullptr;
}

APawn* AFPGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (LastActiveCheckpoint)
	{
		const FTransform SpawnTransform = LastActiveCheckpoint->GetSpawnPointTransform();
		return SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
	}
	
	if (const FFPLevelData* CurrentLevelData = GetGameInstance()->GetSubsystem<USaveGameSubsystem>()->GetCurrentLevelData())
	{
		if (!CurrentLevelData->LastActiveCheckpointID.IsNone() &&
			!CurrentLevelData->LastCheckpointSpawnPoint.Equals(FTransform::Identity))
		{
			return SpawnDefaultPawnAtTransform(NewPlayer, CurrentLevelData->LastCheckpointSpawnPoint);
		}
	}
	
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
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
		
		if(!LastActiveCheckpoint)
		{
			LastActiveCheckpoint = FindCheckpointByID(LastCheckpointID);
		}
		
		if(LastActiveCheckpoint)
		{
			SpawnTransform = LastActiveCheckpoint->GetSpawnPointTransform();
		}
		else
		{
			const AActor* PlayerStartActor = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
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
	if(InCheckpoint && GetWorld())
	{
		InCheckpoint->SetCheckpointActivated(true);
		
		LastActiveCheckpoint = InCheckpoint;
		LastCheckpointID = InCheckpoint->GetSaveID_Implementation();

		if(USaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
		{
			SaveGameSubsystem->SaveGameSlot(SaveGameSubsystem->GetCurrentSlotName());
		}
	}
}

void AFPGameMode::InitGameFromSave()
{
	if(const USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
	{
		LastCheckpointID = SaveSubsystem->GetCurrentLastCheckpointID();
		LastCheckpointSpawnPoint = SaveSubsystem->GetCurrentLastCheckpointSpawnPoint();
		LastActiveCheckpoint = FindCheckpointByID(LastCheckpointID);
		SaveSubsystem->LoadCurrentLevelFromSave();
	}
}


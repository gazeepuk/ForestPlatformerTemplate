// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"

#include "EngineUtils.h"
#include "EnvironmentActors/Checkpoints/FPCheckpoint.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/SavableActorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SaveGameSubsystem.h"


FTransform AFPGameMode::GetLastCheckpointSpawnPoint() const
{
	const AFPCheckpoint* LastCheckpoint = FindCheckpointByID(LastCheckpointID);
	return LastCheckpoint ? LastCheckpoint->GetSpawnPointTransform() : FTransform::Identity;
}

void AFPGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all checkpoints and add them to CheckpointsMap
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
	// Spawn a pawn with the LastActiveCheckpoint's spawn point transform
	if (LastActiveCheckpoint)
	{
		const FTransform SpawnTransform = LastActiveCheckpoint->GetSpawnPointTransform();
		return SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
	}

	// Spawn a pawn with saved check point transform
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
	
	UClass* PlayerPawnClass = DefaultPawnClass;

	// Cache Player's pawn class and destroy the pawn
	if(APawn* PlayerPawn = InPlayerController->GetPawn())
	{
		PlayerPawnClass = PlayerPawn->GetClass();
		
		PlayerPawn->Destroy();
	}
	
	FTransform SpawnTransform;
	
	if(!LastActiveCheckpoint)
	{
		LastActiveCheckpoint = FindCheckpointByID(LastCheckpointID);
	}

	// Getting the spawn transform:
	if(LastActiveCheckpoint)
	{
		// Set the spawn transform as LastActiveCheckpoint's transform
		SpawnTransform = LastActiveCheckpoint->GetSpawnPointTransform();
	}
	else
	{
		// Set the spawn transform either as a PlayerStart, if it exists, or FTransform::Identity
		const AActor* PlayerStartActor = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
		SpawnTransform = PlayerStartActor ? PlayerStartActor->GetActorTransform() : FTransform::Identity;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn a new Pawn and posses it
	if(APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PlayerPawnClass, SpawnTransform, SpawnParameters))
	{
		InPlayerController->Possess(SpawnedPawn);
	}
	
}

void AFPGameMode::RegisterCheckpoint(AFPCheckpoint* InCheckpoint)
{
	if(InCheckpoint && GetWorld())
	{
		// Set the checkpoint activated 
		InCheckpoint->SetCheckpointActivated(true);

		// Cache the checkpoint
		LastActiveCheckpoint = InCheckpoint;
		LastCheckpointID = InCheckpoint->GetSaveID_Implementation();

		// Save current process
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
		// Get and cache checkpoint data from save data
		LastCheckpointID = SaveSubsystem->GetCurrentLastCheckpointID();
		LastCheckpointSpawnPoint = SaveSubsystem->GetCurrentLastCheckpointSpawnPoint();
		LastActiveCheckpoint = FindCheckpointByID(LastCheckpointID);
		
		SaveSubsystem->LoadCurrentLevelFromSave();

		RespawnPlayer(GetWorld()->GetFirstPlayerController());
	}
}


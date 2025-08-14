// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"

#include "EngineUtils.h"
#include "Checkpoints/FPCheckpoint.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/SavableObjectInterface.h"
#include "SaveGame/FPSaveGame.h"
#include "Subsystems/SaveGameSubsystem.h"

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
	
	InitGameFromSave();
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

			SpawnTransform = LastCheckpoint ? (*LastCheckpoint)->GetSpawnPointTransform() : FTransform::Identity;
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
	if(InCheckpoint && GetWorld())
	{
		InCheckpoint->SetCheckpointActivated(true);
		
		LastActiveCheckpoint = InCheckpoint;
		LastCheckpointTransform = InCheckpoint->GetActorTransform();
		LastCheckpointID = InCheckpoint->GetSaveID_Implementation();

		GetGameInstance()->GetSubsystem<USaveGameSubsystem>()->SaveGameSlot();
	}
}

void AFPGameMode::InitGameFromSave()
{
	if(!GetWorld())
	{
		return;
	}

	if(USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
	{
		SaveSubsystem->LoadCurrentLevelFromSave();
		if(FFPLevelData* CurrentLevelData = SaveSubsystem->GetCurrentLevelData())
		{
			LastCheckpointID = CurrentLevelData->LastActiveCheckpointID;
		}
	}
}


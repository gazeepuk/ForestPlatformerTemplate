// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCheckpoint.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "CoreTypes/FPCustomCollisions.h"
#include "GameModes/FPGameMode.h"


AFPCheckpoint::AFPCheckpoint()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CheckpointBoundaries = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointBoundaries"));
	CheckpointBoundaries->SetupAttachment(GetRootComponent());
	CheckpointBoundaries->SetGenerateOverlapEvents(true);
	CheckpointBoundaries->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckpointBoundaries->SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);

	CheckpointBoundaries->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPlayerOverlappedBoundaries);
	
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(GetRootComponent());
	SpawnDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnDirectionArrow"));
	SpawnDirectionArrow->SetupAttachment(SpawnPoint);
}

void AFPCheckpoint::SetCheckpointActivated(bool bActivate)
{
	bActivated = bActivate;
}

FName AFPCheckpoint::GetSaveID_Implementation() const
{
	return SaveID;
}

bool AFPCheckpoint::ShouldSave_Implementation() const
{
	return true;
}

FFPSavableData AFPCheckpoint::GetSaveData_Implementation() const
{
	FFPSavableData Data;
	Data.ObjectType = "Checkpoint";
	Data.bActive = bActivated;
	
	return Data;
}

void AFPCheckpoint::LoadFromSaveData_Implementation(const FFPSavableData& SaveData)
{
	bActivated = SaveData.bActive;

	ISavableObjectInterface::Execute_OnLoadedFromSaveData(this);
}

void AFPCheckpoint::OnPlayerOverlappedBoundaries(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bActivated)
	{
		return;
	}
	
	if(AFPGameMode* FPGameMode = GetWorld()->GetAuthGameMode<AFPGameMode>())
	{
		FPGameMode->RegisterCheckpoint(this);
	}
	
	OnCheckpointActivated();
}

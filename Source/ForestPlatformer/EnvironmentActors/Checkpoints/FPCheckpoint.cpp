// Developed by Ivan Piankouski. All Rights Reserved


#include "FPCheckpoint.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "CoreTypes/CustomCollisions/FPCustomCollisions.h"
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

void AFPCheckpoint::SetCheckpointActivated(bool bNewActivated)
{
	if(bActivated == bNewActivated)
	{
		return;
	}
	
	bActivated = bNewActivated;

	if(bActivated)
	{
		OnCheckpointActivated();
	}
}

FTransform AFPCheckpoint::GetSpawnPointTransform() const
{
	if(SpawnPoint)
	{
		return SpawnPoint->GetComponentTransform();
	}
	return FTransform::Identity;
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

	ISavableActorInterface::Execute_OnLoadedFromSaveData(this);
}

void AFPCheckpoint::OnPlayerOverlappedBoundaries(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AFPGameMode* FPGameMode = GetWorld()->GetAuthGameMode<AFPGameMode>())
	{
		FPGameMode->RegisterCheckpoint(this);
	}
}

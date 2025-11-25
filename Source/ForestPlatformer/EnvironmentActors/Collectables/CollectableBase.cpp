// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableBase.h"

#include "Components/BoxComponent.h"
#include "CoreTypes/FPCustomCollisions.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SaveGameSubsystem.h"


ACollectableBase::ACollectableBase()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetCollisionObjectType(ECC_FP_Collectable_OC);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);
	BoxCollision->SetupAttachment(GetRootComponent());
	
	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxCollisionBeginOverlap);

	bActive = true;
}

FFPSavableData ACollectableBase::GetSaveData_Implementation() const
{
	FFPSavableData SavableData;
	SavableData.bActive = bActive;
	SavableData.ObjectType = "Collectable";
	return SavableData;
}

void ACollectableBase::LoadFromSaveData_Implementation(const FFPSavableData& SaveData)
{
	bActive = SaveData.bActive;

	Execute_OnLoadedFromSaveData(this);
}

void ACollectableBase::OnLoadedFromSaveData_Implementation()
{
	if(!bActive)
	{
		if(bDestroyOnCollect)
		{
			Destroy();
		}
		else
		{
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
		}
	}
}

void ACollectableBase::NativeOnCollected(AActor* InInstigator)
{
	if(CollectionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CollectionSound, GetActorLocation(), GetActorRotation(), CollectionSoundVolume);
	}
}


void ACollectableBase::HandleCollection()
{
	bActive = false;
	
	if(bDestroyOnCollect)
	{
		if(bSaveOnCollected)
		{
			if(USaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
			{
				SaveGameSubsystem->AddPendingSavableActor(this);
			}
		}
		
		Destroy();
	}
	else
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		if(bEnableCollectableInTime)
		{
			GetWorldTimerManager().ClearTimer(CollectableRespawnTimerHandle);
			GetWorldTimerManager().SetTimer(CollectableRespawnTimerHandle, this, &ThisClass::RespawnCollectable, CollectableRespawnTime);
		}
	}
}


bool ACollectableBase::CanCollect_Implementation(AActor* InInstigator)
{
	return true;
}

void ACollectableBase::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(CanCollect(OtherActor))
	{
		NativeOnCollected(OtherActor);
		BP_OnCollected(OtherActor);
		HandleCollection();
	}
}


void ACollectableBase::RespawnCollectable()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	bActive = true;
}

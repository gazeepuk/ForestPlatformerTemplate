// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActorBase.h"


APooledActorBase::APooledActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APooledActorBase::BeginPlay()
{
	Super::BeginPlay();

	SetActive(false);
}

void APooledActorBase::SetActive(bool InActive)
{
	GetWorldTimerManager().ClearTimer(DeactivateActorTimerHandle);

	if(InActive)
	{
		ActivateActor();
	}
	else
	{
		DeactivateActor();
	}
}

void APooledActorBase::ActivateActor_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	PrimaryActorTick.SetTickFunctionEnable(true);

	GetWorldTimerManager().SetTimer(DeactivateActorTimerHandle, this, &ThisClass::DeactivateActor, ActiveLifeTime);
}

void APooledActorBase::DeactivateActor_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	PrimaryActorTick.SetTickFunctionEnable(false);
}

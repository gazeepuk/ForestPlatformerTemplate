// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActorBase.h"


APooledActorBase::APooledActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APooledActorBase::SetPooledActorActive(bool InActive)
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

void APooledActorBase::OnActorActivated_Implementation()
{
}

void APooledActorBase::OnActorDeactivated_Implementation()
{
}

void APooledActorBase::ActivateActor()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bActive = true;
	
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	GetWorldTimerManager().SetTimer(DeactivateActorTimerHandle, this, &ThisClass::DeactivateActor, ActiveLifeTime);

	OnActorActivated();
}

void APooledActorBase::DeactivateActor()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	bActive = false;
	
	PrimaryActorTick.SetTickFunctionEnable(false);

	OnActorDeactivated();
}

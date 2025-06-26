// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructiblePlatform.h"

#include "ActorComponents/CollisionComponents/StompCollisions/StompBoxComponent.h"


ADestructiblePlatform::ADestructiblePlatform()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootScene")));
	
	StompBoxComponent = CreateDefaultSubobject<UStompBoxComponent>(TEXT("StompBoxComponent"));
	StompBoxComponent->SetupAttachment(GetRootComponent());
	StompBoxComponent->OnStompBoxOverlappedActorAbove.AddDynamic(this, &ThisClass::OnStompBoxOverlapped);
}

void ADestructiblePlatform::OnStompBoxOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bDestructing)
	{
		bDestructing = true;
		GetWorldTimerManager().SetTimer(DestructionTimerHandle, this, &ThisClass::DestroyPlatform, DestructionTime);
	}
}

void ADestructiblePlatform::DestroyPlatform()
{
	if(bDestroyAfter)
	{
		Destroy();
		return;
	}

	if(GetWorld())
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		
		GetWorldTimerManager().SetTimer(RestorationTimerHandle, this, &ThisClass::RestorePlatform, RestorationTime);
	}
}

void ADestructiblePlatform::RestorePlatform()
{
	bDestructing = false;
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	GetWorldTimerManager().ClearTimer(DestructionTimerHandle);
	GetWorldTimerManager().ClearTimer(RestorationTimerHandle);
}

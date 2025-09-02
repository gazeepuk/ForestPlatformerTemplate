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
	// Destroy or disable the platform if it's active
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
		// Disable the platform and
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		// Set a timer to restore the platform
		GetWorldTimerManager().SetTimer(RestorationTimerHandle, this, &ThisClass::RestorePlatform, RestorationTime);
	}
}

void ADestructiblePlatform::RestorePlatform()
{
	bDestructing = false;

	// Enable the platform
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// Clear all timers
	GetWorldTimerManager().ClearTimer(DestructionTimerHandle);
	GetWorldTimerManager().ClearTimer(RestorationTimerHandle);
}

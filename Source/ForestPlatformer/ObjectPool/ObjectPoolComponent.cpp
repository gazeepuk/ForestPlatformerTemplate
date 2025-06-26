// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"

#include "PooledActorBase.h"


APooledActorBase* UObjectPoolComponent::FindFirstAvailableActor() const
{
	APooledActorBase* const* FoundPoolActor = ObjectPool.FindByPredicate([](const APooledActorBase* PooledActor)
	{
		return PooledActor && !PooledActor->IsActive();
	});
	
	return *FoundPoolActor;
}

APooledActorBase* UObjectPoolComponent::SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform)
{
	APooledActorBase* SpawnedPoolActor = FindFirstAvailableActor();
	if(!SpawnedPoolActor && bCanExpandPool)
	{
		SpawnedPoolActor = SpawnPoolActor(InActorSpawnTransform, true);
	}
	
	if(SpawnedPoolActor)
	{
		SpawnedPoolActor->SetActorTransform(InActorSpawnTransform);
		SpawnedPoolActor->SetActive(true);
		return SpawnedPoolActor;
	}
	
	return nullptr;
}

void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializePool();
}

APooledActorBase* UObjectPoolComponent::SpawnPoolActor(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn)
{
	if(UWorld* World = GetWorld())
	{
		FVector SpawnLocation = InActorSpawnTransform.GetLocation();
		FRotator SpawnRotation = InActorSpawnTransform.GetRotation().Rotator();
		
		if(APooledActorBase* SpawnedPooledActor = World->SpawnActor<APooledActorBase>(PooledActorClass, SpawnLocation, SpawnRotation))
		{
			SpawnedPooledActor->SetActive(bActivateOnSpawn);
			ObjectPool.AddUnique(SpawnedPooledActor);
			return SpawnedPooledActor;
		}
	}

	return nullptr;
}

void UObjectPoolComponent::InitializePool()
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		SpawnPoolActor(FTransform::Identity);
	}
}

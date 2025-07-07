// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"

#include "PooledActorBase.h"


void UObjectPoolContainer::InitializePool(UObject* InWorldContext, AActor* InOwningActor)
{
	WorldContext = InWorldContext;

	if(!PooledActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pooled Actor Class is not valid. Can't initialize object pool"));
	}
	if(PoolSize <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool size is %d. Can't initialize object pool"), PoolSize);
	}

	OwningActor = InOwningActor;
	
	for (int32 i = 0; i < PoolSize; ++i)
	{
		SpawnPoolActor(FTransform::Identity);
	}
}

APooledActorBase* UObjectPoolContainer::FindFirstAvailableActor() const
{
	APooledActorBase* const* FoundPoolActor = ObjectPool.FindByPredicate([](const APooledActorBase* PooledActor)
	{
		return PooledActor && !PooledActor->IsActive();
	});
	
	return FoundPoolActor ? *FoundPoolActor : nullptr;
}

APooledActorBase* UObjectPoolContainer::SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform, bool bActivateActor)
{
	APooledActorBase* SpawnedPoolActor = FindFirstAvailableActor();
	if(!SpawnedPoolActor && bCanExpandPool)
	{
		SpawnedPoolActor = SpawnPoolActor(InActorSpawnTransform, bActivateActor);
	}
	
	if(SpawnedPoolActor)
	{
		SpawnedPoolActor->SetActorTransform(InActorSpawnTransform);
		SpawnedPoolActor->SetPooledActorActive(bActivateActor);
		return SpawnedPoolActor;
	}
	
	return nullptr;
}

APooledActorBase* UObjectPoolContainer::SpawnPoolActor(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn)
{
	if(!PooledActorClass)
	{
		return nullptr;
	}
	
	if(UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr)
	{
		if(APooledActorBase* SpawnedPooledActor = World->SpawnActorDeferred<APooledActorBase>(PooledActorClass, InActorSpawnTransform, OwningActor, Cast<APawn>(OwningActor)))
		{
			SpawnedPooledActor->SetPooledActorActive(bActivateOnSpawn);
			ObjectPool.AddUnique(SpawnedPooledActor);
			SpawnedPooledActor->FinishSpawning(InActorSpawnTransform);
			return SpawnedPooledActor;
		}
	}

	return nullptr;
}


// UObjectPoolComponent
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
		SpawnedPoolActor->SetPooledActorActive(true);
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
			SpawnedPooledActor->SetPooledActorActive(bActivateOnSpawn);
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

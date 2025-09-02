// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"

#include "PooledActorBase.h"

#pragma region ObjectPoolContainer
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

void UObjectPoolContainer::ClearPool()
{
	if(ObjectPool.IsEmpty())
	{
		return;
	}
	
	for (APooledActorBase* PooledActor : ObjectPool)
	{
		PooledActor->Destroy();
	}

	ObjectPool.Empty();
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
		PoolSize++;
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
#pragma endregion

#pragma region ObjectPoolComponent
#pragma endregion
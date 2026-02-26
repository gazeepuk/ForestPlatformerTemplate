// Developed by Ivan Piankouski. All Rights Reserved


#include "ObjectPoolComponent.h"

#include "PooledActorBase.h"

DEFINE_LOG_CATEGORY(LogFpObjectPool);

#pragma region ObjectPoolContainer
UWorld* UObjectPoolContainer::GetWorld() const
{
	return WorldContext ? WorldContext->GetWorld() : nullptr;
}

void UObjectPoolContainer::InitializePool(UObject* InWorldContext, AActor* InOwningActor)
{
	WorldContext = InWorldContext;

	if(!PooledActorClass)
	{
		UE_LOG(LogFpObjectPool, Warning, TEXT("Pooled Actor Class is not valid. Can't initialize object pool"));
	}
	if(PoolSize <= 0)
	{
		UE_LOG(LogFpObjectPool, Warning, TEXT("Pool size is not valid (%d). Can't initialize object pool"), PoolSize);
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
		SpawnedPoolActor = SpawnPoolActor(InActorSpawnTransform, false);
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
	
	if(UWorld* World = GetWorld())
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

UObjectPoolContainer* UObjectPoolComponent::GetObjectPoolContainer() const
{
	return ObjectPoolContainer;
}

APooledActorBase* UObjectPoolComponent::SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform,
	bool bActivateOnSpawn)
{
	if(ObjectPoolContainer)
	{
		return ObjectPoolContainer->SpawnPoolActorFromPool(InActorSpawnTransform, bActivateOnSpawn);
	}
	return nullptr;
}

void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ObjectPoolContainer)
	{
		ObjectPoolContainer->InitializePool(this, GetOwner());
	}
}

void UObjectPoolComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ObjectPoolContainer->ClearPool();
	ObjectPoolContainer->ConditionalBeginDestroy();
	
	Super::EndPlay(EndPlayReason);
}

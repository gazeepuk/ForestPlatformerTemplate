// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

class APooledActorBase;

UCLASS(BlueprintType, EditInlineNew)
class UObjectPoolContainer : public UObject
{
	GENERATED_BODY()

public:
	/* Spawn actors of pool and deactivate them */
	void InitializePool(UObject* InWorldContext, AActor* InOwningActor);
	/* Destroy all pooled actors and clear the pool array */
	void ClearPool();

	/**
	 * Find first available pooled actor from the pool
	 */
	UFUNCTION(BlueprintPure)
	APooledActorBase* FindFirstAvailableActor() const;
	/*
	 * Find a disabled actor or create a new one, if bCanExpand is true, and activate it.
	 */
	UFUNCTION(BlueprintCallable)
	APooledActorBase* SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform, bool bActivateActor = true);
	
protected:
	/* Spawn a new Actor with specific transform */
	APooledActorBase* SpawnPoolActor(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn = false);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledActorBase> PooledActorClass;

	/* Default PoolSize */
	UPROPERTY(EditAnywhere)
	int32 PoolSize;

	/* Can the pool be expanded if there is no available pooled actor */
	UPROPERTY(EditAnywhere)
	bool bCanExpandPool = true;

	/* The pool of actors */
	UPROPERTY()
	TArray<APooledActorBase*> ObjectPool;
	
	UPROPERTY()
	TObjectPtr<UObject> WorldContext;

	/* Pool's owner */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpObjectPool, Log, All);

class APooledActorBase;

UCLASS(BlueprintType, EditInlineNew)
class UObjectPoolContainer : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;
	
	/* Spawns actors of pool and deactivate them */
	void InitializePool(UObject* InWorldContext, AActor* InOwningActor);
	/* Destroys all pooled actors and clear the pool array */
	void ClearPool();

	/**
	 * Finds first available pooled actor from the pool
	 */
	UFUNCTION(BlueprintPure)
	APooledActorBase* FindFirstAvailableActor() const;
	/*
	 * Finds a disabled actor or create a new one, if bCanExpand is true, and activate it.
	 */
	UFUNCTION(BlueprintCallable)
	APooledActorBase* SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform, bool bActivateActor = true);
	
protected:
	/* Spawns a new Actor with specific transform */
	APooledActorBase* SpawnPoolActor(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn = false);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledActorBase> PooledActorClass;

	/* Default PoolSize */
	UPROPERTY(EditAnywhere)
	int32 PoolSize;

	/* Determines whether the pool can be expanded if there is no available pooled actor */
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

/**
 * A component designed to use object pool container for efficient actors management
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Returns object pool container
	 */
	UFUNCTION(BlueprintPure)
	UObjectPoolContainer* GetObjectPoolContainer() const;

	/**
	 * Spawns an actor if the pool is expandable
	 */
	UFUNCTION(BlueprintCallable)
	APooledActorBase* SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn = false);
	
protected:
	/**
	 * Used for the pool initialization
	 */
	virtual void BeginPlay() override;
	/**
	 * Used for clearing the pool
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UObjectPoolContainer> ObjectPoolContainer;
};

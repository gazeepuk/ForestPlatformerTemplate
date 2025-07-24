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
	void InitializePool(UObject* InWorldContext, AActor* InOwningActor);
	void ClearPool();
	
	UFUNCTION(BlueprintPure)
	APooledActorBase* FindFirstAvailableActor() const;
	UFUNCTION(BlueprintCallable)
	APooledActorBase* SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform, bool bActivateActor = true);
	
protected:
	APooledActorBase* SpawnPoolActor(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn = false);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledActorBase> PooledActorClass;
	
	UPROPERTY(EditAnywhere)
	int32 PoolSize;
	
	UPROPERTY(EditAnywhere)
	bool bCanExpandPool = true;

	UPROPERTY()
	TArray<APooledActorBase*> ObjectPool;

	UPROPERTY()
	TObjectPtr<UObject> WorldContext;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	APooledActorBase* FindFirstAvailableActor() const;
	APooledActorBase* SpawnPoolActorFromPool(const FTransform& InActorSpawnTransform);
	
protected:
	virtual void BeginPlay() override;

	APooledActorBase* SpawnPoolActor(const FTransform& InActorSpawnTransform, bool bActivateOnSpawn = false);
		
private:
	void InitializePool();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APooledActorBase> PooledActorClass;
	
	UPROPERTY(EditDefaultsOnly)
	int32 PoolSize;
	
	UPROPERTY(EditDefaultsOnly)
	bool bCanExpandPool = true;
	
	TArray<APooledActorBase*> ObjectPool;
};

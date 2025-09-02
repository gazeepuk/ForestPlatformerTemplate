// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActorBase.generated.h"

UCLASS()
class FORESTPLATFORMER_API APooledActorBase : public AActor
{
	GENERATED_BODY()

public:
	/* Default constructor for APooledActorBase */
	APooledActorBase();

	/* Set the actor active or not */
	UFUNCTION(BlueprintCallable, Category = "PooledActor")
	void SetPooledActorActive(bool InActive);

	/* Return bActive */
	UFUNCTION(BlueprintPure, Category = "PooledActor")
	FORCEINLINE bool IsActive() const { return bActive; }
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void ActivateActor();
	
	UFUNCTION(BlueprintNativeEvent)
	void DeactivateActor();

private:
	/* Lifetime after activation */
	UPROPERTY(EditDefaultsOnly, Category = "PooledActor")
	float ActiveLifeTime = 10.f;
	
	bool bActive = true;

	FTimerHandle DeactivateActorTimerHandle;
};

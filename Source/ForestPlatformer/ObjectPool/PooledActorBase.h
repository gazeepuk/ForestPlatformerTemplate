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
	APooledActorBase();
	
	UFUNCTION(BlueprintCallable)
	void SetActive(bool InActive);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsActive() const { return bActive; }
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void ActivateActor();
	
	UFUNCTION(BlueprintNativeEvent)
	void DeactivateActor();

private:
	UPROPERTY(EditDefaultsOnly)
	float ActiveLifeTime = 10.f;
	
	bool bActive = false;

	FTimerHandle DeactivateActorTimerHandle;
};

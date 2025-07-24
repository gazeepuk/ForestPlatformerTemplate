// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FPAsyncLaunchActor.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchCompleted);
UCLASS()
class FORESTPLATFORMER_API UFPAsyncLaunchActor : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnLaunchCompleted OnLaunchCompleted;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly))
	static UFPAsyncLaunchActor* LaunchActor(UObject* WorldContextObject, AActor* InLaunchingActor, FVector LaunchDirection, float Distance = 300.f, float MaxHeight = 300.f, float Duration = 1.f, bool bOrientToMovement = false);
	
	virtual void Activate() override;
	virtual UWorld* GetWorld() const override { return WorldContext.IsValid() ? WorldContext->GetWorld() : nullptr; }
private:
	void TickMovement();
	void Cleanup();

	TWeakObjectPtr<UObject> WorldContext;
	TWeakObjectPtr<AActor> LaunchingActor;
	FVector StartLocation;
	FVector TargetLocation;
	FVector ControlPoint;
	float ElapsedTime;
	float TotalDuration;
	bool bShouldOrientToMovement;
	FTimerHandle LaunchTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FPAsyncLaunchActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchCompleted);
/**
 * Asynchronous blueprint node for launching actors parabolic trajectory.
 * Provides smooth arc movement with configurable parameters
 */
UCLASS()
class FORESTPLATFORMER_API UFPAsyncLaunchActor : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Delegate that broadcasts when the launch movement has finished */
	UPROPERTY(BlueprintAssignable)
	FOnLaunchCompleted OnLaunchCompleted;

	/**
	 * Creates an async action to launch an actor along a parabolic path 
	 * @param WorldContextObject World context for getting the world reference 
	 * @param InLaunchingActor The actor to be launched
	 * @param LaunchDirection Direction vector for the launch movement 
	 * @param Distance Total horizontal distance to travel
	 * @param MaxHeight Maximin height of the parabolic path
	 * @param Duration Total time duration of the launch sequence
	 * @param bOrientToMovement Whether to rotate the actor to face movement direction 
	 * @return Configure async launch action
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly))
	static UFPAsyncLaunchActor* LaunchActor(UObject* WorldContextObject, AActor* InLaunchingActor, FVector LaunchDirection, float Distance = 300.f, float MaxHeight = 300.f, float Duration = 1.f, bool bOrientToMovement = false);

	/** Activates the async launch operation */
	virtual void Activate() override;
	/** Returns the world context for this async action */
	virtual UWorld* GetWorld() const override { return WorldContext.IsValid() ? WorldContext->GetWorld() : nullptr; }
private:
	/** Updates the actor's transform each frame during the launch */
	void TickMovement();
	/** Cleans up when the launch sequence completes or cancelled */
	void Cleanup();

	/** Weak reference to the world context object for save world access */
	TWeakObjectPtr<UObject> WorldContext;
	/** Weak reference to the actor being launched */
	TWeakObjectPtr<AActor> LaunchingActor;
	/** Starting location of the actor when the launch began */
	FVector StartLocation;
	/** Target destination location for the launch */
	FVector TargetLocation;
	/** Control point for defining parabolic arc trajectory */
	FVector ControlPoint;
	/** Elapsed time since the launch sequence started */
	float ElapsedTime;
	/** Total duration of the launch sequence */
	float TotalDuration;
	/** Whether the actor should rotate to face its movement direction during launch */
	bool bShouldOrientToMovement;
	/** Timer handle for managing the launch update tick */
	FTimerHandle LaunchTimerHandle;

	bool bFalling;
	FVector CurrentVelocity;
	
	void ParabolicMovement();
	void FallingMovement();
	void SwitchToFalling();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"

/**
 * Component that manages patrol behavior for AI characters,
 * providing waypoint navigation and random movement
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Checks if the current patrol index points to a valid patrol point */
	UFUNCTION(BlueprintPure)
	bool IsCurrentPatrolIndexValid() const;
	/** Checks if the patrol borders volume are valid */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsPatrolBordersValid() const { return PatrolBorders != nullptr; } 

	/** Returns the world location of a patrol point by its index */
	UFUNCTION(BlueprintPure)
	FVector GetPatrolPointLocationByIndex(int32 InPatrolPointIndex) const;
	/** Returns the world of the current patrol point */
	UFUNCTION(BlueprintPure)
	FVector GetCurrentPatrolPointLocation() const;

	/** Returns a random location within the patrol borders volume */
	UFUNCTION(BlueprintPure)
	FVector GetRandomLocationWithinPatrolBorders() const;
	
	/** Advances to the next patrol point in the sequence */
	UFUNCTION(BlueprintCallable)
	void SetNextPatrolPointIndex();
	
protected:
	virtual void BeginPlay() override;

	/** Array of actor waypoints that define the patrol route */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> PatrolPoints;

	/** Index of the current patrol point in the PatrolPoints array */
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPatrolPointIndex = -1;

	/**	Direction of patrol progression (true = forward, false = backward ) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPatrolForward = true;

	/** Volume that defines the boundaries for random patrol movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AVolume> PatrolBorders;
};

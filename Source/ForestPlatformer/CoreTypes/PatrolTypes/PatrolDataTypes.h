// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PatrolDataTypes.generated.h"

/**
 * Base class for data-driven patrol definitions.
 *
 * Used by custom UBTTask_GetNextPatrolPoint to determine how an AI pawn
 * selects its next patrol location. Make a subclass of this class to define a new patrol type.
 * Make sure your subclasses have BlueprintType and EditInlineNew tags.
 */
UCLASS(Blueprintable, Abstract)
class FORESTPLATFORMER_API UPatrolDataBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Returns the next patrol location for the given pawn
	 * 
	 * @param PatrollingPawn The pawn requesting a patrol location
	 * @param OutNextLocation The found patrol location
	 * @return True if a valid location was found, False otherwise
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool GetNextPatrolLocation(APawn* PatrollingPawn, FVector& OutNextLocation) const;
};

/**
 * Chooses a random location within defined bounds. Patrolling pawn must have UPatrolComponent
 */
UCLASS(BlueprintType, EditInlineNew)
class UPatrolInBounds : public UPatrolDataBase
{
	GENERATED_BODY()

public:
	virtual bool GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const override;
};

/**
 * Follows the sequence of patrol points in order. Patrolling pawn must have UPatrolComponent
 */
UCLASS(BlueprintType, EditInlineNew)
class UPatrolSequence : public UPatrolDataBase
{
	GENERATED_BODY()

public:
	virtual bool GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const override;
};

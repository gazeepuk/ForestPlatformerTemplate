// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PatrolDataTypes.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class FORESTPLATFORMER_API UPatrolDataBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool GetNextPatrolLocation(APawn* PatrollingPawn, FVector& OutNextLocation) const;
};

UCLASS(BlueprintType, EditInlineNew)
class UPatrolInBoundsData : public UPatrolDataBase
{
	GENERATED_BODY()

public:
	virtual bool GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const override;
};

UCLASS(BlueprintType, EditInlineNew)
class UPatrolSequence : public UPatrolDataBase
{
	GENERATED_BODY()

public:
	virtual bool GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const override;
};

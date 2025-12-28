// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/EnumTypes/FPEnumTypes.h"
#include "UObject/Object.h"
#include "ObserverCondition.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpObserverCondition, Log, All);

class UObservableState;

/**
 * Conditions are used for checking observable states.
 * They check the state values with the expected values and return the checking result.
 */

/**
 * Base condition class
 */
UCLASS(Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class FORESTPLATFORMER_API UObserverCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool IsConditionMet(UObservableState* InObservableState) const;
};

/**
 * Bool condition. It's used for checking UBoolObservableState value 
 */
UCLASS(BlueprintType, EditInlineNew)
class UBoolObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bExpectedValue = true;
	
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

/**
 * Trigger Condition. It's used for checking UTriggerObservableState value 
 */
UCLASS(BlueprintType, EditInlineNew)
class UTriggerObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

UCLASS(BlueprintType, EditInlineNew)
class UFloatObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float ExpectedValue = 0.f;

	UPROPERTY(EditAnywhere)
	EFPComparisonType ComparisonType = EFPComparisonType::EqualsTo;
	
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

UCLASS(BlueprintType, EditInlineNew)
class UInt32ObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 ExpectedValue = 0;

	UPROPERTY(EditAnywhere)
	EFPComparisonType ComparisonType = EFPComparisonType::EqualsTo;
	
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

UCLASS(BlueprintType, EditInlineNew)
class UVectorObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector ExpectedValue;
	
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};
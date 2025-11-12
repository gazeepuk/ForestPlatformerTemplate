// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
UCLASS(Blueprintable, EditInlineNew)
class UBoolObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bExpectedValue = true;
	
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class UTriggerObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

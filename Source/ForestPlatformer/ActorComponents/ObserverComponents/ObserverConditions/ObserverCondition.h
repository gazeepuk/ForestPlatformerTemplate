// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ObserverCondition.generated.h"

class UObservableState;
/**
 * 
 */
UCLASS(Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class FORESTPLATFORMER_API UObserverCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool IsConditionMet(UObservableState* InObservableState) const;
};

UCLASS(Blueprintable, EditInlineNew)
class UBoolObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bExpectedValue = true;
	
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

UCLASS(Blueprintable, EditInlineNew)
class UTriggerObserverCondition : public UObserverCondition
{
	GENERATED_BODY()

public:
	virtual bool IsConditionMet_Implementation(UObservableState* InObservableState) const override;
};

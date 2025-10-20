// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InstancedStruct.h"
#include "ObservableState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChanged);
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class FORESTPLATFORMER_API UObservableState : public UObject
{
	GENERATED_BODY()

public:
	FOnStateChanged OnStateChanged;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FInstancedStruct GetStateValue();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetStateValue(const FInstancedStruct& NewValue);
};

USTRUCT()
struct FMyStruct
{
	GENERATED_BODY()
	
};

UCLASS(BlueprintType, EditInlineNew)
class UBoolObservableState : public UObservableState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetStateBoolValue(bool bNewValue);
	
	UFUNCTION(BlueprintPure)
	bool GetStateBoolValue();

	virtual FInstancedStruct GetStateValue_Implementation() override;

	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

protected:
	UPROPERTY(EditAnywhere)
	bool bValue;
};

UCLASS(BlueprintType, EditInlineNew)
class UToggleObservableState : public UBoolObservableState
{
	GENERATED_BODY()

public:
	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;
	
	void ToggleState();	
};

UCLASS(BlueprintType, EditInlineNew)
class UTriggerObservableState : public UObservableState
{
	GENERATED_BODY()

public:
	
	virtual FInstancedStruct GetStateValue_Implementation() override;
	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

	UFUNCTION(BlueprintCallable)
	void ResetTrigger();
	
	UFUNCTION(BlueprintCallable)
	void TriggerState();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasTriggered() const { return bHasTriggered; }

protected:
	UPROPERTY(EditAnywhere)
	bool bCanTriggerAgain = true;
	
private:
	bool bHasTriggered = false;
};

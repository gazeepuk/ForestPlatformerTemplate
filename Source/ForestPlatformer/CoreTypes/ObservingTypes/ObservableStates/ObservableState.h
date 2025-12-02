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


/**
 * Base observable state with. Broadcasts a delegate after changing the value
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

/**
 * Bool observable state.
 * - Stores a bool value.
 * - You can use SetStateBoolValue and GetStateBoolValue for convenience
 */
UCLASS(BlueprintType, EditInlineNew)
class UBoolObservableState : public UObservableState
{
	GENERATED_BODY()

public:
	/**
	 * Sets value of this state. A convenient way to change value, if you have a cast state of this type
	 */
	UFUNCTION(BlueprintCallable)
	void SetStateBoolValue(bool bNewValue);
	
	UFUNCTION(BlueprintPure)
	bool GetStateBoolValue() const;

	virtual FInstancedStruct GetStateValue_Implementation() override;

	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

protected:
	UPROPERTY(EditAnywhere)
	bool bValue;
};

/**
 * Toggle observable state.
 * - A child class of UBoolObservableState
 * - Can't set directly the bool state, but toggle it.
 * - The value of NewValue input param in SetStateValue() does not matter.
 * - You can use ToggleState for convenient toggling.
 */
UCLASS(BlueprintType, EditInlineNew)
class UToggleObservableState : public UBoolObservableState
{
	GENERATED_BODY()

public:
	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

	UFUNCTION(BlueprintCallable)
	void ToggleState();	
};

/**
 * Trigger observable state.
 * - Broadcast changes whenever the state is set.
 * - Tracks whether it has already triggered.
 * - Optionally allows repeated triggering.
 * - Can be reset via ResetTrigger()
 */
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

/**
 * Float observable state.
 * - Stores a float value.
 * - You can use SetStateFloatValue and GetStateFloatValue for convenience
 */
UCLASS(BlueprintType, EditInlineNew)
class UFloatObservableState : public UObservableState
{
	GENERATED_BODY()

public:
	/**
	 * Sets value of this state. A convenient way to change value, if you have a cast state of this type
	 */
	UFUNCTION(BlueprintCallable)
	void SetStateFloatValue(float InNewValue);
	
	UFUNCTION(BlueprintPure)
	float GetStateFloatValue() const;

	virtual FInstancedStruct GetStateValue_Implementation() override;

	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

protected:
	UPROPERTY(EditAnywhere)
	float Value;
};

/**
 * Int32 observable state.
 * - Stores a int32 value.
 * - You can use SetStateInt32Value and GetStateInt32Value for convenience
 */
UCLASS(BlueprintType, EditInlineNew)
class UInt32ObservableState : public UObservableState
{
	GENERATED_BODY()

public:
	/**
	 * Sets value of this state. A convenient way to change value, if you have a cast state of this type
	 */
	UFUNCTION(BlueprintCallable)
	void SetStateInt32Value(int32 InNewValue);
	
	UFUNCTION(BlueprintPure)
	int32 GetStateInt32Value() const;

	virtual FInstancedStruct GetStateValue_Implementation() override;

	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

protected:
	UPROPERTY(EditAnywhere)
	int32 Value;
};

/**
 * Vector observable state.
 * - Stores a FVector value.
 * - You can use SetStateVectorValue and GetStateVectorValue for convenience
 */
UCLASS(BlueprintType, EditInlineNew)
class UVectorObservableState : public UObservableState
{
	GENERATED_BODY()

public:
	/**
	 * Sets value of this state. A convenient way to change value, if you have a cast state of this type
	 */
	UFUNCTION(BlueprintCallable)
	void SetStateVectorValue(FVector InNewValue);
	
	UFUNCTION(BlueprintPure)
	FVector GetStateVectorValue() const;

	virtual FInstancedStruct GetStateValue_Implementation() override;

	virtual void SetStateValue_Implementation(const FInstancedStruct& NewValue) override;

protected:
	UPROPERTY(EditAnywhere)
	FVector Value;
};
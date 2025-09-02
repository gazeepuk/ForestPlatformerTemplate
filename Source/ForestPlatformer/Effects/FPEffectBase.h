// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPEffectBase.generated.h"


UENUM(BlueprintType)
enum class EFPEffectDurationType : uint8
{
	Instant,	// Effect applies and does not persist
	Duration,	// Effect applies and persists for specified duration 
	Infinite	// Effect applies and persists until manually removed 
};

UENUM(BlueprintType)
enum class EFPEffectStackingPolicy : uint8
{
	NoStack,					// Cannot stack if an effect with the same ID is already applied
	ApplyAsSeparatedEffect,		// Apply as a separate effect even if same ID exists 
	RefreshDuration,			// Refresh the duration of existing effect with same ID 
	AddToRemainingTime			// Add to the remaining duration of existing effect with same ID
};
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class FORESTPLATFORMER_API UFPEffectBase : public UObject
{
	GENERATED_BODY()

public:
	/** Override GetWorld() to provide proper world context */
	virtual UWorld* GetWorld() const override { return WorldContextObject ? WorldContextObject->GetWorld() : nullptr; }

	/** Sets the world context object of this effect */
	void SetWorldContextObject(UObject* InWorldContextObject) { WorldContextObject = InWorldContextObject; }

	/** Returns the unique identifier for this effect type */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE FName GetEffectID() const { return EffectID; }

	/** Returns the hashed version of the effect ID for efficient comparisons */
	FORCEINLINE uint32 GetEffectIDHash() const {return EffectIDHash;}

	/** Determines if the effect can be applied to the target actor */
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	bool CanApply(AActor* Target) const;

	/** Contains the main effect logic that executes when the effect is applied */
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnApplied(AActor* Target);

	/** Contains the main effect logic that executes before the effect is removed */
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnRemoved(AActor* Target);

	/** Contains logic that executes every tick for duration-based effects */
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnTick(AActor* Target, float DeltaTime);

	/** Returns the duration type of this effect */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE EFPEffectDurationType GetDurationType() const { return DurationType; }

	/** Returns the duration of this effect */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE float GetDuration() const { return Duration; }

	/** Returns the stacking policy for this effect*/
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE EFPEffectStackingPolicy GetStackingPolicy() const { return StackingPolicy; }
	
protected:
	/** Unique identifier for this effect type. Each child class should have a distinct ID */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FName EffectID;

	/** Hashed EffectID for efficient comparison*/
	uint32 EffectIDHash;

	/** Determines how this effect persists over time */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect|Duration")
	EFPEffectDurationType DurationType = EFPEffectDurationType::Duration;

	/** Duration of the effect in seconds (only relevant for Duration type effects) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect|Duration")
	float Duration = 5.0f;

	/** Determines how multiple instances of the same effect interact */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	EFPEffectStackingPolicy StackingPolicy = EFPEffectStackingPolicy::NoStack;
	
#if WITH_EDITOR
	/** Handle property changes in te editor, particularly for the EffectID and EffectIDHashed */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:
	/** Reference to the world context object for proper world access */
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;
};
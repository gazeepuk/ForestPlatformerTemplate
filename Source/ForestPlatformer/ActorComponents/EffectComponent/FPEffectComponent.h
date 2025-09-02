// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Effects/FPEffectBase.h"
#include "FPEffectComponent.generated.h"

class UFPEffectBase;

/** Structure representing an active effect instance with timing information */
USTRUCT(BlueprintType)
struct FFPActiveEffect
{
	GENERATED_BODY()

	/** The effect instance currently active */
	UPROPERTY()
	TObjectPtr<UFPEffectBase> Effect = nullptr;

	/** Remaining duration for duration-based effects */
	float RemainingTime = 0.0f;

	/** Timer handle for managing effect expiration */
	FTimerHandle TimerHandle;

	/** Checks if this active effect is valid */
	bool IsValid() const { return Effect && !Effect->GetEffectID().IsNone(); }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UFPEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPEffectComponent();

	/** Attempts to apply an effect to this component */
	UFUNCTION(BlueprintCallable, Category = "Effects")
	bool TryApplyEffect(UFPEffectBase* Effect);

	/** Removes the specified effect instance from this component */
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffect(const UFPEffectBase* Effect);

	/** Removes an effect of the specified class from this component */
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffectByClass(const TSubclassOf<UFPEffectBase> InEffectClass);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Array of all active effects */
	UPROPERTY()
	TArray<FFPActiveEffect> ActiveEffects;

private:
	/** Called when an effect's duration expires */
	void OnEffectExpired(const UFPEffectBase* Effect);
	/** Handles effect stacking policies when applying new effects */
	void HandleStacking(const UFPEffectBase* InEffect, bool& bOutShouldApplyEffect);
};

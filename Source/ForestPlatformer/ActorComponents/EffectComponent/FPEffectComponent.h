// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Effects/FPEffectBase.h"
#include "FPEffectComponent.generated.h"

class UFPEffectBase;

USTRUCT(BlueprintType)
struct FFPActiveEffect
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UFPEffectBase> Effect = nullptr;

	float RemainingTime = 0.0f;
	
	FTimerHandle TimerHandle;
	
	bool IsValid() const { return Effect && !Effect->GetEffectID().IsNone(); }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UFPEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPEffectComponent();

	UFUNCTION(BlueprintCallable, Category = "Effects")
	bool TryApplyEffect(UFPEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffect(const UFPEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffectByClass(const TSubclassOf<UFPEffectBase> InEffectClass);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	TArray<FFPActiveEffect> ActiveEffects;

private:
	void OnEffectExpired(const UFPEffectBase* Effect);
	void HandleStacking(const UFPEffectBase* InEffect, bool& bOutShouldApplyEffect);
};

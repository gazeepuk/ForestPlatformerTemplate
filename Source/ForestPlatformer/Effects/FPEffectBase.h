// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPEffectBase.generated.h"


UENUM(BlueprintType)
enum class EFPEffectDurationType : uint8
{
	Instant,
	Duration,
	Infinite
};
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class FORESTPLATFORMER_API UFPEffectBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE FName GetEffectID() const { return EffectID; }

	FORCEINLINE uint32 GetEffectIDHash() const {return EffectIDHash;}
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	bool CanApply(AActor* Target) const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnApplied(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnRemoved(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnTick(AActor* Target, float DeltaTime);
	
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE EFPEffectDurationType GetDurationType() const { return DurationType; }

	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE float GetDuration() const { return Duration; }

protected:
	virtual void PostInitProperties() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FName EffectID;

	uint32 EffectIDHash;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	EFPEffectDurationType DurationType = EFPEffectDurationType::Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta = (EditCondition = "DurationType == EFPEffectDurationType::Duration"))
	float Duration = 5.0f;
};
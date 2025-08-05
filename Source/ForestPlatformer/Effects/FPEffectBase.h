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

UENUM(BlueprintType)
enum class EFPEffectStackingPolicy : uint8
{
	NoStack,
	RefreshDuration,
	AddToRemainingTime
};
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class FORESTPLATFORMER_API UFPEffectBase : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override { return WorldContextObject ? WorldContextObject->GetWorld() : nullptr; }

	void SetWorldContextObject(UObject* InWorldContextObject) {};
	
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

	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE EFPEffectStackingPolicy GetStackingPolicy() const { return StackingPolicy; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FName EffectID;

	uint32 EffectIDHash;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	EFPEffectDurationType DurationType = EFPEffectDurationType::Duration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (EditCondition = "DurationType == EFPEffectDurationType::Duration", ExposeOnSpawn = "true"))
	float Duration = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	EFPEffectStackingPolicy StackingPolicy = EFPEffectStackingPolicy::NoStack;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;
};
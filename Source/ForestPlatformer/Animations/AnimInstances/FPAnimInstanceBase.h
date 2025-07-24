// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "FPAnimInstanceBase.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UFPAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag InGameplayTagToCheck) const;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	TObjectPtr<ACharacter> OwningCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	float LocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	FVector Velocity;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	bool bFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	bool bHasAcceleration;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
enum class EFPMovementState : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Strafe	UMETA(DisplayName = "Strafe")
};
/**
 * Enhanced character movement component with custom speed modification
 * and floating movement mechanic
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UFPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual float GetMaxSpeed() const override;

	/** Sets speed multiplier for the character's movement speed */
	UFUNCTION(BlueprintCallable)
	void SetSpeedMultiplier(float InSpeedMultiplier);
	/** Returns current speed multiplier */
	UFUNCTION(BlueprintPure)
	float GetSpeedMultiplier() const;

	/** Activates floating */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Movement|Floating")
	void StartFloating();
	/** Deactivates floating */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Movement|Floating")
	void StopFloating();
	/** Activates or deactivates floating based on character state */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Movement|Floating") 
	void HandleFloating();

	UFUNCTION(BlueprintCallable, Category = "Character Movement|MovementState")
	void SetCurrentMovementState(EFPMovementState InMovementState);
	UFUNCTION(BlueprintPure, Category = "Character Movement|MovementState")
	EFPMovementState GetCurrentMovementState() const;

	/** Returns walk speed of the specified state */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Character Movement|MovementState")
	float GetMovementStateSpeed(EFPMovementState InMovementState) const;
	/** Returns walk speed of the current state */
	UFUNCTION(BlueprintPure, Category = "Character Movement|MovementState")
	float GetCurrentMovementStateSpeed() const;
protected:
	/** Gravity multiplier applied specifically during floating movement */
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Floating")
	float FloatingGravityMultiplier = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: MovementState")
	float NormalSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: MovementState")
	float StrafeSpeed = 300.f;

	/** Handles movement state changes. Sets new speed, orienting rotation */
	UFUNCTION(BlueprintNativeEvent)
	void HandleMovementState();
private:
	EFPMovementState CurrentMovementState = EFPMovementState::Normal;
	
	/** Speed multiplier affecting all movement speed */
	float SpeedMultiplier = 1.f;

	/** Indicates whether the character is currently in floating movement mode */
	bool bFloating;

	/** Cached gravity value before floating start. Used for restoring the original gravity after ending floating */
	float CachedGravity;
};

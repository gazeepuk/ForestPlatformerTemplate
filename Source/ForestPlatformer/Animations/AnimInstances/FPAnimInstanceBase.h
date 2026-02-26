// Developed by Ivan Piankouski. All Rights Reserved

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
	/** Initializes animations references */
	virtual void NativeInitializeAnimation() override;
	/** Updates animations variables */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	/**
	 * Checks if the owning actor has the specified tag
	 * @param InGameplayTagToCheck The gameplay tag to verify
	 * @return True if the owner has the tag, false otherwise
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag InGameplayTagToCheck) const;

	/** Reference to the character that owns this animation instance */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	TObjectPtr<ACharacter> OwningCharacter;

	/** Reference to the character movement component */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;

	/** Current ground speed of the character */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	float GroundSpeed;

	/** Direction of locomotion relative to character facing*/
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	float LocomotionDirection;

	/** Current velocity vector of the character */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	FVector Velocity;

	/** Indicates whether the character should be moving */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	bool bShouldMove;

	/** Indicates whether the character is falling or in air */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	bool bFalling;

	/** Indicates whether the character has acceleration */
	UPROPERTY(BlueprintReadOnly, Category = "AnimData")
	bool bHasAcceleration;
};

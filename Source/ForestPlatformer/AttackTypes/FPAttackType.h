// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPAttackType.generated.h"

class UCombatComponentBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FORESTPLATFORMER_API UFPAttackType : public UObject
{
	GENERATED_BODY()

public:
	/** Returns the world context fo this attack type */
	UFUNCTION(BlueprintPure)
	virtual UWorld* GetWorld() const override;

	/** Returns the unique identifier for this attack type */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetAttackTypeID() const {return GetClass()->GetFName();}

	/** Return the hashed version of the attack type ID for efficient comparisons */
	FORCEINLINE uint32 GetAttackTypeIDHash() const {return GetTypeHash(GetAttackTypeID());}

	/** Returns the gameplay tag associated with this attack type */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FGameplayTag GetAttackTypeTag() const {return AttackTypeTag;}

	/** Returns true if the attack is active */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsActive() const { return bActive; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsOnCooldown() const { return RemainingCooldown > 0.f; }
	
	/** Sets the actor that owns this attack type */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

	/** Returns the actor that owns this attack type */
	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetOwningActor() const { return OwningActor; }

	/** Sets the combat component of this attack type */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCombatComponent(UCombatComponentBase* InCombatComponent) { CombatComponent = InCombatComponent; }

	/** Returns the combat component of this attack type */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UCombatComponentBase* GetCombatComponent() const { return CombatComponent; }

	/** Returns the character from the owning actor */
	UFUNCTION(BlueprintPure)
	ACharacter* GetCharacterFromOwningActor() const;

	/** Returns the skeletal mesh from the owning actor */
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetSkeletalMeshFromOwningActor() const;

	/**
	 * Initialize the attack type with ownership and combat component
	 * @param InOwningActor The actor that own and perform this attack type
	 * @param InCombatComponent The combat component managing this attack
	 */
	UFUNCTION(BlueprintCallable)
	virtual void InitAttack(AActor* InOwningActor, UCombatComponentBase* InCombatComponent);

	/**
	 * Activates the attack against the specified target actor
	 */
	UFUNCTION(BlueprintCallable)
	void PerformAttack();
	
	/**
	 * Contain the main attack logic
	 */
	UFUNCTION(BlueprintNativeEvent)
	void PerformAttackInner();

	/**
	 * Determines if the attack can be performed by the owner
	 * @return True if the attack can be performed, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAttack();

	/**
	 * Ends the current attack and cleans up
	 */
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	/**
	 * Contains cleanup logic (clears timers, stops montages). Override to properly clean up the attack
	 */
	UFUNCTION(BlueprintNativeEvent)
	void EndAttackInner();

	/**
	 * Aborts this attack
	 * @return True if aborted successfully
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAbortAttack();
	
	/** Delegate that broadcasts when the attack sequence has completed */
	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;
	
protected:

	/** Animation montage to play when performing this attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Gameplay tag used to identify this attack type */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackTypeTag;

private:
	UFUNCTION(BlueprintCallable)
	void StopAttackMontage(float BlendOut = 0.1f);
	
	/** Starts attack cooldown */
	void StartCooldown();
	/** Resets the cooldown */
	void ResetCooldown();
	/** Updates the remaining cooldown time of this attack */
	void UpdateCooldown();

	/** The actor that owns and perform this attack type */
	UPROPERTY()
	TObjectPtr<AActor> OwningActor;

	/** The combat component that manages this attack type */
	UPROPERTY()
	TObjectPtr<UCombatComponentBase> CombatComponent;
	
	/** Indicates whether the attack is currently active */
	bool bActive = false;

	/** Indicates whether the attack can be interrupted */
	UPROPERTY(EditDefaultsOnly)
	bool bInterruptible = false;

	/** Ability's cooldown time in seconds */
	UPROPERTY(EditDefaultsOnly)
	float AttackCooldown = 0.f;

	/** The remaining time of cooldown in seconds */
	float RemainingCooldown;

	float CooldownTick = 0.1f;
	
	/** timer handle for cooldown */
	FTimerHandle CooldownTimerHandle;
};

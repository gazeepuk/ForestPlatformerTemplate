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
	FORCEINLINE FName GetAttackTypeID() const {return AttackTypeID;}

	/** Return the hashed version of the attack type ID for efficient comparisons */
	FORCEINLINE uint32 GetAttackTypeIDHash() const {return AttackTypeIDHash;}

	/** Returns the gameplay tag associated with this attack type */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FGameplayTag GetAttackTypeTag() const {return AttackTypeTag;}

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
	 * Performs the attack against the specified target actor
	 * @param InTargetActor The actor to attack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformAttack(AActor* InTargetActor);

	/**
	 * Determines if the attack can be performed by the owner
	 * @return True if the attack can be performed, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAttack();

	/** Ends the current attack and cleans up */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndAttack();

	/** Delegate that broadcasts when the attack sequence has completed */
	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;
	
protected:
	/** Called after all properties are initialized to set up the attack identifiers */
	virtual void PostInitProperties() override;

	/** Finds valid targets for this attack type within range and conditions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<AActor*> FindTarget();

	/** Animation montage to play when performing this attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Gameplay tag used to identify this attack type */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackTypeTag;

private:
	/** Unique identifier for this attack type */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FName AttackTypeID;
	
	/** Hashed ID of this attack type for efficient comparisons */
	uint32 AttackTypeIDHash;

	/** The actor that owns and perform this attack type */
	UPROPERTY()
	TObjectPtr<AActor> OwningActor;

	/** The combat component that manages this attack type */
	UPROPERTY()
	TObjectPtr<UCombatComponentBase> CombatComponent;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponentBase.h"
#include "Components/ActorComponent.h"
#include "PlayerCombatComponent.generated.h"


struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UFPAttackType;

/**
 * Player-specific combat component that handles input binding, attack management for player-controlled characters
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPlayerCombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()

public:

	/** Checks if the player can currently perform an attack */
	virtual bool CanAttack_Implementation() override;

	/** Attempts to perform the currently set primary attack */
	bool TryPerformCurrentAttack();

	/** Sets the primary attack type of the specified class */
	UFUNCTION(BlueprintCallable)
	void SetCurrentAttackByClass(TSubclassOf<UFPAttackType> InPrimaryAttackClass);

	/** Returns a tag associated with the current attack */
	UFUNCTION(BlueprintPure)
	FGameplayTag GetCurrentAttackTag() const;
	
	/** Sets the primary attack type associated with the specified tag */
	UFUNCTION(BlueprintCallable)
	void SetCurrentAttackByTag(FGameplayTag InAttackTag);
	
	/** Resets the primary attack back to the default attack type */
	UFUNCTION(BlueprintCallable)
	void ResetPrimaryAttack();
	/** Initialize combat component, binding inputs, setting the default attack */
	UFUNCTION(BlueprintCallable)
	void InitCombatComponent();

	/** Unbinds input for the owner player controller */
	UFUNCTION(BlueprintCallable)
	void UnbindCombatInput();
	
	/** Returns the current attack type */
	UFUNCTION(BlueprintPure)
	UFPAttackType* GetCurrentAttackType();
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/** Input mapping context containing combat-related input bindings */
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Input")
	TObjectPtr<UInputMappingContext> CombatMappingContext;

	/** Input action that triggers the primary attack */
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Input")
	TObjectPtr<UInputAction> AttackAction;

	/** Priority level of the combat input mapping context */
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Input")
	int32 CombatInputPriority = 2;

	/** Callback function triggered when the attack input action is started */
	UFUNCTION()
	void AttackAction_Started(const FInputActionValue& ActionValue);

	/** Executes the attack */
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteAttack();
	
	/** Default attack type class used when no custom attack is set */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|AttackTypes")
	TSubclassOf<UFPAttackType> DefaultAttackType;

	/** Default attack's gameplay tag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|AttackTypes")
	FGameplayTag DefaultAttackTag;
	
	/** Current attack's gameplay tag */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|AttackTypes")
	FGameplayTag CurrentAttackTag;
	
private:
	/** Binds combat input actions to the player's input component */
	void BindCombatInput();
};

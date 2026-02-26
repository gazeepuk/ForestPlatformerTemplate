// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponentBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpCombat, Log, All);

struct FGameplayTag;
class UFPAttackType;

/**
 * Base component for combat functionality providing attack management,
 * state tracking, and attack type handling through gameplay tags 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UCombatComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Checks if the owner can currently perform an attack
	 * @note Always call parent function in derived classes!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAttack();

	/** Returns the currently active attack */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UFPAttackType* GetActiveAttack() const { return ActiveAttack; }
	
	/** Attempts to perform an attack associated with the specified tag */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryActivateAttackByTag(const FGameplayTag& InAttackTypeTag);

	/** Initializes default attack for this component */
	UFUNCTION(BlueprintCallable)
	void InitAttacks();

	/** Grants an attack type of the specified class */
	UFUNCTION(BlueprintCallable)
	UFPAttackType* GrantAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass);
	/** Removes an attack type of the specified class */
	UFUNCTION(BlueprintCallable)
	void RemoveAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass);

	/**
	 * Finds an attack type of the specified class
	 * @param InAttackTypeClass The class of an attack type to be found
	 */
	UFUNCTION(BlueprintPure)
	UFPAttackType* FindAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass);

	/** Searches and returns the attack type associated with the specified gameplay tag */
	UFUNCTION(BlueprintPure)
	UFPAttackType* FindAttackTypeByTag(const FGameplayTag& InAttackTypeTag);

	/**
	 * Aborts the active attack
	 * @return True if aborted successfully
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAbortActiveAttack();

	UFUNCTION(BlueprintPure)
	bool CanActivateAttackByTag(FGameplayTag InAttackTypeTag);

	UFUNCTION(BlueprintPure)
	float GetAttackRemainingCooldown(FGameplayTag InAttackTypeTag);
protected:
	
	/** Callback function triggered when an attack has ended */
	UFUNCTION()
	virtual void OnAttackEnded();
	
	/** Array of the available attack types */
	UPROPERTY()
	TArray<UFPAttackType*> AvailableAttackTypes;

	/** Default attack types to be granted */
	UPROPERTY(EditDefaultsOnly, Category = "Combat|AttackTypes")
	TArray<TSubclassOf<UFPAttackType>> DefaultAttackTypeClasses;

	/** Activates the specified ability */
	UFUNCTION(BlueprintNativeEvent)
	bool TryActivateAttack(UFPAttackType* InAttackTypeToActivate);
	
	UFUNCTION(BlueprintPure)
	int32 GetIndexOfAttack(UFPAttackType* InAttackType) const;
private:

	/** The current active attack */
	TObjectPtr<UFPAttackType> ActiveAttack;
};

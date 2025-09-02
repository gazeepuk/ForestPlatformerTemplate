// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamageDelegate, float, DamageValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	/** Determines if the owner is alive */
	UFUNCTION(BlueprintPure, Category = "Health Component")
	bool IsAlive() const;
	/** Returns current health value */
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth; }
	/** Return maximum health value */
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	/** Returns true if current health is equal to or greater than max health, false otherwise */
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE bool IsHealthFull() const { return CurrentHealth >= MaxHealth; }

	/** Sets the maximum health value (clamps minimal value to 1.f) */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SetMaxHealth(float InNewMaxHealth);
	/** Sets the current health value (clamped between 0,f and max health) */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SetCurrentHealth(float InNewCurrentHealth);
	/** Restores health by the specified amount */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void RestoreCurrentHealth(float InHealthAmount);
	/** Reduces health by the specified amount */
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SpendCurrentHealth(float InHealthAmount);

	/** Applies damage to the health component
	 * @note Always call Super::TakeDamage_Implementation in derived classes!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Component")
	void TakeDamage(AActor* DamageCauser, float InDamageValue, AController* InstigatedBy);

	/** Delegate that broadcasts when the current health reaches zero */
	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthDelegate OnZeroHealth;
	/** Delegate that broadcasts when the current health changes */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChanged;
	/** Delegate that broadcasts when the damage is taken (after health changed) */
	UPROPERTY(BlueprintAssignable)
	FOnTakeDamageDelegate OnTakeDamage;
	
protected:
	virtual void PostInitProperties() override;

	/** Maximum health value that can be achieved */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 3.f;
	/** Current health value (clamped between 0.f and MaxHealth) */
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;
};

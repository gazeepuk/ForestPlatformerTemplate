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
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth; }
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintPure, Category = "Health Component")
	FORCEINLINE bool IsHealthFull() const { return CurrentHealth >= MaxHealth; }
	
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SetCurrentHealth(float InNewCurrentHealth);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void RestoreCurrentHealth(float InHealthAmount);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SpendCurrentHealth(float InHealthAmount);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void TakeDamage(AActor* DamageCauser, float InDamageValue, AController* InstigatedBy);

	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthDelegate OnZeroHealth;
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnTakeDamageDelegate OnTakeDamage;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 3.f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;
};

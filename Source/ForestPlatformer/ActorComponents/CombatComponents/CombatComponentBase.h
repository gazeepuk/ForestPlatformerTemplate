// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponentBase.generated.h"


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
	bool CanAttack() const;

	/** Attempts to perform an attack associated with the specified tag */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryAttackByTag(const FGameplayTag& InAttackTypeTag);
	
protected:
	
	/** Callback function triggered when an attack has ended */
	UFUNCTION()
	virtual void OnAttackEnded();
};

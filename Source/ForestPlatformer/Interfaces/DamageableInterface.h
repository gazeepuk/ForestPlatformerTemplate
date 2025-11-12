// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can receive damage.
 *
 * Any class implementing this interface must provide logic for handling incoming damage events.
 * This allows a unified way to apply damage across different actor types (characters, destructible objects, etc.)
 */
class FORESTPLATFORMER_API IDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Applies the specified amount of damage */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(AActor* DamageCauser, float InDamage, AController* InstigatedBy);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoinsWalletInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCoinsWalletInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing a coin wallet system.
 * This interfaces allows systems (shops, pickups) to interact with coins in a unified way.
 */
class FORESTPLATFORMER_API ICoinsWalletInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/** Adds the specified number of coins to the wallet */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddCoins(int32 InAmount);
	/** Attempts to spend the specified number of coins from the wallet */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TrySpendCoins(int32 InAmount);
	/** Check if there is the specified number of coins in the wallet */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasEnoughCoins(int32 InCoinsNumToSpend) const;
	/** Directly sets the specified number of coins in the wallet */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentCoins(int32 InNewCurrentCoins);
	/** Returns the current number of coins in the wallet */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetCurrentCoins() const;
};

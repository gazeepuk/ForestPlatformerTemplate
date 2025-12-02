// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WalletComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCurrentCoinsChanged, int32, CurrentCoins, int32, MaxCoins, int32, DeltaCoins);

/** A wallet component that manages player's money */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UWalletComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Adds the specified amount of coins to the current amount */
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void AddCoins(int32 InAmount);

	/** Spends the current coins, if it has enough coins */
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SpendCoins(int32 InAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SetCurrentCoins(int32 InCoins);

	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SetMaxCoins(int32 InMaxCoins);
	
	/** Returns true, if the current coins amount is greater or equals to the specified value */
	UFUNCTION(BlueprintPure, Category = "Coins")
	bool HasEnoughCoins(int32 InCoinsNumInSpend) const;

	/** Returns current coins amount */
	UFUNCTION(BlueprintPure, Category = "Coins")
	int32 GetCurrentCoins() const;

	/** Returns the maximum coins amount that player can bear*/
	UFUNCTION(BlueprintPure, Category = "Coins")
	int32 GetMaxCoins() const;
	
	/** Broadcasts after changing the current coins amount */
	UPROPERTY(BlueprintAssignable, Category = "Coins")
	FOnCurrentCoinsChanged OnCurrentCoinsChanged;

	/** Sets whether the coins amount can be negative or not. Clamps the value after changing, if the new value is false */
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SetCanBeNegative(bool bNewCanBeNegative);

	/** Sets whether the coins amount must be clamped or not. Clamps the value after changing, if the new value is true */
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SetClampingMaxValue(bool bNewClampCurrentCoins);

private:
	
	/** If true, the current amount of coins can be negative */
	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	bool bCanBeNegative = false;
	/** If true, clamps the current amount of coins, when it gets changed */
	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	bool bClampMaxValue = true;
	
	/** The current amount of coins */
	int32 CurrentCoins;
	/** The maximum amount of coins that player can bear */
	int32 MaxCoins = INT32_MAX;
};

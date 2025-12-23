// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/CoinsWalletInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "FPPlayerController.generated.h"

class UWalletComponent;
class UInventoryComponent;
class UHealthComponent;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API AFPPlayerController : public APlayerController, public ICoinsWalletInterface, public IGenericTeamAgentInterface, public IInventoryInterface
{
	GENERATED_BODY()
public:
	AFPPlayerController();
	
	//~Begin IGenericTeamAgentInterface
	/** Returns the team ID associated with the player */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/** Determines the team attitudes towards another actor */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End IGenericTeamAgentInterface

	//~Begin IInventoryInterface
	/**
	 * Returns the amount of the item in the inventory
	 * @param InItemName The unique name of the inventory item
	 */
	virtual int32 GetItemCount_Implementation(FName InItemName) override;
	/** Adds an item to the inventory */
	virtual bool AddItem_Implementation(UObject* InInventoryObject, int32 InQuantity) override;
	/** Removes an item from the inventory */
	virtual bool RemoveItem_Implementation(UObject* InInventoryObject, int32 InQuantity) override;
	//~End IInventoryInterface
	
	//~Begin ICoinsWalletInterface
	/** Adds the specified number of coins to the player's wallet */
	virtual void AddCoins_Implementation(int32 InCoinsNum) override;
	/** Attempts to spend the specified number of coins from the player's wallet */
	virtual bool TrySpendCoins_Implementation(int32 InCoinsNum) override;
	/** Check if the player has at least the specified number of coins */
	virtual bool HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const override;
	/** Directly sets the specified number of coins in the player's wallet */
	virtual void SetCurrentCoins_Implementation(int32 InNewCurrentCoins) override;
	/** Returns the current number of coins in the player's wallet */
	virtual int32 GetCurrentCoins_Implementation() const override;
	//~End ICoinsWalletInterface


	/** Initialize HUD widget for the player */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitHUDWidget();
	
	/** Initialize the health UI with provided health component */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitHealthBar(UHealthComponent* InHealthComponent);

protected:
	/** Inventory component. Stores and manages player's inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Wallet component. Stores and manages player's money */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWalletComponent> WalletComponent;
	
private:
	/** Team identifier for this player controller */
	FGenericTeamId PlayerTeamID;
};
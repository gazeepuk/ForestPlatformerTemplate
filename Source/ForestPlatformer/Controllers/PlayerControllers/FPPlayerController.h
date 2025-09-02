// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/CoinsWalletInterface.h"
#include "FPPlayerController.generated.h"

class UHealthComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentCoinsChanged, int32, CurrentCoins);
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API AFPPlayerController : public APlayerController, public ICoinsWalletInterface, public IGenericTeamAgentInterface
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

	/** Delegate hat broadcasts whenever the player's current coin count changes */
	UPROPERTY(BlueprintAssignable)
	FOnCurrentCoinsChanged OnCurrentCoinsChangedDelegate;
	
private:
	/** Team identifier for this player controller */
	FGenericTeamId PlayerTeamID;

	/** Current number of coins the player possesses */
	int32 CurrentCoins;

	/** Maximum number of coins the player can carry */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxCoins = INT32_MAX;
};

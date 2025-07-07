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
	
	// Begin IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// End IGenericTeamAgentInterface
	
	// Begin ICoinsWalletInterface
	virtual void AddCoins_Implementation(int32 InCoinsNum) override;
	virtual bool TrySpendCoins_Implementation(int32 InCoinsNum) override;
	virtual bool HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const override;
	virtual void SetCurrentCoins_Implementation(int32 InNewCurrentCoins) override;
	virtual int32 GetCurrentCoins_Implementation() const override;
	// End ICoinsWalletInterface

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnCurrentCoinsChanged(int32 NewCurrentCoins);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitHealthBar(UHealthComponent* InHealthComponent);
	
	UPROPERTY(BlueprintAssignable)
	FOnCurrentCoinsChanged OnCurrentCoinsChangedDelegate;
	
private:
	FGenericTeamId PlayerTeamID;
	
	int32 CurrentCoins;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxCoins = INT32_MAX;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinCollectableBase.h"

#include "Interfaces/CoinsWalletInterface.h"


void ACoinCollectableBase::NativeOnCollected(AActor* InInstigator)
{
	if(InInstigator && InInstigator->Implements<UCoinsWalletInterface>())
	{
		ICoinsWalletInterface::Execute_AddCoins(InInstigator, CoinsValue);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinCollectableBase.h"

#include "Interfaces/CoinsWalletInterface.h"


void ACoinCollectableBase::OnCollectableOverlapped_Implementation(AActor* InOverlappedActor)
{
	if(InOverlappedActor && InOverlappedActor->Implements<UCoinsWalletInterface>())
	{
		ICoinsWalletInterface::Execute_AddCoins(InOverlappedActor, CoinsValue);

		Super::OnCollectableOverlapped_Implementation(InOverlappedActor);
	}
}

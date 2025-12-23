// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerController.h"

#include "ActorComponents/InventoryComponent/InventoryComponent.h"
#include "ActorComponents/WalletComponent/WalletComponent.h"
#include "Interfaces/InventoryItemInterface.h"

AFPPlayerController::AFPPlayerController()
{
	PlayerTeamID = FGenericTeamId(0);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	WalletComponent = CreateDefaultSubobject<UWalletComponent>(TEXT("WalletComponent"));
}

FGenericTeamId AFPPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

ETeamAttitude::Type AFPPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if(!OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}
	
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());
	if(OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

int32 AFPPlayerController::GetItemCount_Implementation(FName InItemName)
{
	if(InventoryComponent)
	{
		return InventoryComponent->GetItemCountByID(InItemName);
	}

	return 0;
}

bool AFPPlayerController::AddItem_Implementation(UObject* InInventoryObject, int32 InQuantity)
{
	if(!InInventoryObject || !InInventoryObject->Implements<UInventoryItemInterface>())
	{
		return false;
	}

	TSoftObjectPtr<UInventoryItemDataAsset> InventoryItemDataAsset = IInventoryItemInterface::Execute_GetInventoryItemDataAsset(InInventoryObject);
	if(InventoryComponent && !InventoryItemDataAsset.IsNull())
	{
		if(InventoryComponent->AddItem(InventoryItemDataAsset, InQuantity))
		{
			IInventoryItemInterface::Execute_OnAddedToInventory(InInventoryObject);
			return true;
		}
	}

	return false;
}

bool AFPPlayerController::RemoveItem_Implementation(UObject* InInventoryObject, int32 InQuantity)
{
	if(InventoryComponent)
	{
		return InventoryComponent->RemoveItem(IInventoryItemInterface::Execute_GetInventoryItemDataAsset(InInventoryObject), InQuantity);
	}

	return false;
}

void AFPPlayerController::AddCoins_Implementation(int32 InCoinsNum)
{
	if(WalletComponent)
	{
		WalletComponent->AddCoins(InCoinsNum);
	}
}

bool AFPPlayerController::TrySpendCoins_Implementation(int32 InCoinsNum)
{
	if(WalletComponent && WalletComponent->HasEnoughCoins(InCoinsNum))
	{
		WalletComponent->SpendCoins(InCoinsNum);
		return true;
	}
	return false;
}

bool AFPPlayerController::HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const
{
	return WalletComponent && WalletComponent->HasEnoughCoins(InCoinsNumToSpend);
}

void AFPPlayerController::SetCurrentCoins_Implementation(int32 InNewCurrentCoins)
{
	if(WalletComponent)
	{
		WalletComponent->SetCurrentCoins(InNewCurrentCoins);
	}
}

int32 AFPPlayerController::GetCurrentCoins_Implementation() const
{
	return WalletComponent ? WalletComponent->GetCurrentCoins() : 0;
}


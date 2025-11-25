// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCollectableBase.h"

#include "Interfaces/InventoryInterface.h"


TSoftObjectPtr<UInventoryItemDataAsset> AInventoryCollectableBase::GetInventoryItemDataAsset_Implementation() const
{
	return InventoryItemDataAsset;
}

void AInventoryCollectableBase::NativeOnCollected(AActor* InInstigator)
{
	Super::NativeOnCollected(InInstigator);

	if(InInstigator->Implements<UInventoryInterface>())
	{
		IInventoryInterface::Execute_AddItem(InInstigator, this, InventoryItemQuantity);
	}
}

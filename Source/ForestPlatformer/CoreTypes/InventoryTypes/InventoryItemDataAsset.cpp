// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemDataAsset.h"

TSoftObjectPtr<UInventoryItemDataAsset> UInventoryItemDataAsset::GetInventoryItemDataAsset_Implementation() const
{
	return this;
}

void UInventoryItemDataAsset::OnAddedToInventory_Implementation()
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncLoadInventoryItemUIInfo.h"

#include "CoreTypes/InventoryTypes/InventoryItemDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UAsyncLoadInventoryItemUIInfo* UAsyncLoadInventoryItemUIInfo::AsyncLoadInventoryItemUIInfo(
	const FInventorySlot& InInventorySlot)
{
	UAsyncLoadInventoryItemUIInfo* Node = NewObject<UAsyncLoadInventoryItemUIInfo>();
	Node->InventorySlot = InInventorySlot;
	return Node;
}

void UAsyncLoadInventoryItemUIInfo::Activate()
{
	if(InventorySlot.InventoryItemDataAsset.IsNull())
	{
		OnFailed.Broadcast(FInventoryItem());
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	FSoftObjectPath ItemPath = InventorySlot.InventoryItemDataAsset.ToSoftObjectPath();

	TWeakObjectPtr<UAsyncLoadInventoryItemUIInfo> WeakThis(this);
	StreamableManager.RequestAsyncLoad(ItemPath, FStreamableDelegate::CreateUObject(this, &UAsyncLoadInventoryItemUIInfo::OnInventoryInfoLoaded));
}

void UAsyncLoadInventoryItemUIInfo::OnInventoryInfoLoaded()
{
	if(InventorySlot.InventoryItemDataAsset.IsNull())
	{
		return;
	}
	
	if(UInventoryItemDataAsset* ItemDataAsset = Cast<UInventoryItemDataAsset>(InventorySlot.InventoryItemDataAsset.ToSoftObjectPath().ResolveObject()))
	{
		FInventoryItem InventoryItem;
		InventoryItem.ItemName = ItemDataAsset->GetItemDisplayName();
		InventoryItem.ItemDescription = ItemDataAsset->GetItemDescription();
		InventoryItem.ItemIcon = ItemDataAsset->GetItemTexture();
		InventoryItem.ItemQuantity = InventorySlot.Quantity;
				
		OnSuccess.Broadcast(InventoryItem);
	}
	else
	{
		OnFailed.Broadcast(FInventoryItem());
	}
}

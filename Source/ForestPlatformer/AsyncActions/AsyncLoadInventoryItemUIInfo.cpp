// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncLoadInventoryItemUIInfo.h"

#include "CoreTypes/InventoryTypes/InventoryItemDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UAsyncLoadInventoryItemUIInfo::BeginDestroy()
{
	if(StreamableHandle.IsValid())
	{
		StreamableHandle->CancelHandle();
		StreamableHandle.Reset();
	}

	Super::BeginDestroy();
}

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
	
	
	StreamableHandle = StreamableManager.RequestAsyncLoad(ItemPath, FStreamableDelegate::CreateLambda(
		[ItemPath, WeakThis]()
		{
			if(!WeakThis.IsValid())
			{
				return;
			}
			UAsyncLoadInventoryItemUIInfo* This = WeakThis.Get();
			
			if(UInventoryItemDataAsset* ItemDataAsset = Cast<UInventoryItemDataAsset>(ItemPath.ResolveObject()))
			{
				FInventoryItem InventoryItem;
				InventoryItem.ItemName = ItemDataAsset->GetItemDisplayName();
				InventoryItem.ItemDescription = ItemDataAsset->GetItemDescription();
				InventoryItem.ItemIcon = ItemDataAsset->GetItemTexture();
				InventoryItem.ItemQuantity = This->InventorySlot.Quantity;
				
				This->OnSuccess.Broadcast(InventoryItem);
			}
			else
			{
				This->OnFailed.Broadcast(FInventoryItem());
			}

			This->StreamableHandle.Reset();
		}));
}

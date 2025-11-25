// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "CoreTypes/InventoryTypes/InventoryItemDataAsset.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(TSoftObjectPtr<UInventoryItemDataAsset> InInventoryItemData, int32 InQuantity)
{
	if(InInventoryItemData.IsNull() || InQuantity <= 0)
	{
		return;
	}

	for (FInventorySlot& Slot : InventorySlots)
	{
		if(Slot.InventoryItemDataAsset == InInventoryItemData)
		{
			Slot.Quantity += InQuantity;
			OnInventoryUpdated.Broadcast(InventorySlots);
			return;
		}
	}

	FInventorySlot NewSlot;
	NewSlot.SlotID = FGuid::NewGuid();
	NewSlot.InventoryItemDataAsset = InInventoryItemData;
	NewSlot.Quantity = InQuantity;

	InventorySlots.Add(NewSlot);

	OnInventoryUpdated.Broadcast(InventorySlots);
}

void UInventoryComponent::RemoveItem(TSoftObjectPtr<UInventoryItemDataAsset> InInventoryItemData, int32 InQuantity)
{
	if(InInventoryItemData.IsNull() || InQuantity <= 0)
	{
		return;
	}
	
	for (int i = 0; i < InventorySlots.Num(); ++i)
	{
		if(InventorySlots[i].InventoryItemDataAsset == InInventoryItemData)
		{
			InventorySlots[i].Quantity = FMath::Max(0, InventorySlots[i].Quantity - InQuantity);
			if(InventorySlots[i].Quantity == 0)
			{
				InventorySlots.RemoveAt(i);
				break;
			}
		}
	}
	
	OnInventoryUpdated.Broadcast(InventorySlots);
}

int32 UInventoryComponent::GetItemCountByID(FName InItemID) const
{
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if(Slot.InventoryItemDataAsset.IsNull())
		{
			continue;
		}

		if(UInventoryItemDataAsset* InventoryItemData = Slot.InventoryItemDataAsset.LoadSynchronous())
		{
			if(InventoryItemData->GetItemID() == InItemID)
			{
				return Slot.Quantity;
			}
		}
	}

	return 0;
}

int32 UInventoryComponent::GetItemCountByDataAsset(TSoftObjectPtr<UInventoryItemDataAsset> InInventoryItemData) const
{
	if(!InInventoryItemData)
	{
		return 0;
	}
	
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if(Slot.InventoryItemDataAsset == InInventoryItemData)
		{
			return Slot.Quantity;
		}
	}
	
	return 0;
}

void UInventoryComponent::LoadInventory(TArray<FInventorySlot> InInventory)
{
	InventorySlots = InInventory;
}

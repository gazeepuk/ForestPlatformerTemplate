// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreTypes/InventoryTypes/InventorySlot.h"
#include "InventoryComponent.generated.h"

class UInventoryItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<FInventorySlot>&, UpdatedInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemAdded, FInventorySlot, AddedItemSlot, int32, AddedQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemRemoved, FInventorySlot, RemovedItemSlot, int32, RemovedQuantity);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemAdded OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemRemoved OnInventoryItemRemoved;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<FInventorySlot> GetInventoryCopy() const { return InventorySlots; }
	
	UFUNCTION(BlueprintCallable)
	bool AddItem(TSoftObjectPtr<UInventoryItemDataAsset> InInventoryItemData, int32 InQuantity);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(TSoftObjectPtr<UInventoryItemDataAsset> InInventoryItemData, int32 InQuantity);

	UFUNCTION(BlueprintPure)
	int32 GetItemCountByID(FName InItemID) const;

	UFUNCTION(BlueprintPure)
	int32 GetItemCountByDataAsset(TSoftObjectPtr<UInventoryItemDataAsset> InInventoryItemData) const;

	UFUNCTION(BlueprintCallable)
	void LoadInventory(TArray<FInventorySlot> InInventory);

protected:
	TArray<FInventorySlot> InventorySlots;
	
};

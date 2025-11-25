// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemInterface.generated.h"

class UInventoryItemDataAsset;
// This class does not need to be modified.
UINTERFACE()
class UInventoryItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can be added to an inventory.
 */
class FORESTPLATFORMER_API IInventoryItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Returns Data Asset that represents this item in inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Item")
	TSoftObjectPtr<UInventoryItemDataAsset> GetInventoryItemDataAsset() const;

	// Use for custom behavior after adding this item to the inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Item")
	void OnAddedToInventory();
};

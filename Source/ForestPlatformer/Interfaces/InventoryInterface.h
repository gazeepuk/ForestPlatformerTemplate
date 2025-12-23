// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can access an inventory
 */
class FORESTPLATFORMER_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Returns the amount of the item in the inventory
	 * @param InItemName The unique name of the inventory item
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetItemCount(FName InItemName);

	/** Adds an item to the inventory */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool AddItem(UObject* InInventoryObject, int32 InQuantity);
	/** Returns an item from the inventory */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool RemoveItem(UObject* InInventoryObject, int32 InQuantity);
};

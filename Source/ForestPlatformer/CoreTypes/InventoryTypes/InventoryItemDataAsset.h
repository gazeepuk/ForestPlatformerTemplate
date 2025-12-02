// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interfaces/InventoryItemInterface.h"
#include "InventoryItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UInventoryItemDataAsset : public UDataAsset, public IInventoryItemInterface
{
	GENERATED_BODY()

public:
	virtual TSoftObjectPtr<UInventoryItemDataAsset> GetInventoryItemDataAsset_Implementation() const override;
	virtual void OnAddedToInventory_Implementation() override;
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCENOINLINE FName GetItemID() const { return ItemID; }

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCENOINLINE FText GetItemDisplayName() const { return ItemDisplayName; }

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCENOINLINE FText GetItemDescription() const { return ItemDescription; }
	
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCENOINLINE UTexture2D* GetItemTexture() const { return ItemTexture; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> ItemTexture;
};

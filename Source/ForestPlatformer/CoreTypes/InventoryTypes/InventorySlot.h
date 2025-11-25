#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.generated.h"

class UInventoryItemDataAsset;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot():
		SlotID(FGuid()),
		Quantity(0),
		InventoryItemDataAsset(nullptr)
	{
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid SlotID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UInventoryItemDataAsset> InventoryItemDataAsset;
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()
	
	FInventoryItem():
		ItemName(FText::GetEmpty()),
		ItemDescription(FText::GetEmpty()),
		ItemQuantity(0),
		ItemIcon(nullptr)
	{
	}

	FInventoryItem(FText InItemName, FText InItemDescription, int32 InItemQuantity, TObjectPtr<UTexture2D> InItemIcon):
		ItemName(InItemName),
		ItemDescription(InItemDescription),
		ItemQuantity(InItemQuantity),
		ItemIcon(InItemIcon)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemIcon;
};

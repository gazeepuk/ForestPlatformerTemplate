// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableBase.h"
#include "Interfaces/InventoryItemInterface.h"
#include "InventoryCollectableBase.generated.h"

UCLASS()
class FORESTPLATFORMER_API AInventoryCollectableBase : public ACollectableBase, public IInventoryItemInterface
{
	GENERATED_BODY()

public:

	// ~Begin IInventoryItemInterface
	virtual TSoftObjectPtr<UInventoryItemDataAsset> GetInventoryItemDataAsset_Implementation() const override;
	// ~End IInventoryItemInterface
	
protected:
	virtual void NativeOnCollected(AActor* InInstigator) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Item")
	TSoftObjectPtr<UInventoryItemDataAsset> InventoryItemDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Item")
	int32 InventoryItemQuantity = 1;
};

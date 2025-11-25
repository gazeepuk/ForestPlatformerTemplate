// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/InventoryTypes/InventorySlot.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncLoadInventoryItemUIInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInfoLoaded, const FInventoryItem&, ItemInfo);
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UAsyncLoadInventoryItemUIInfo : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnItemInfoLoaded OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnItemInfoLoaded OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Inventory")
	static UAsyncLoadInventoryItemUIInfo* AsyncLoadInventoryItemUIInfo(const FInventorySlot& InInventorySlot);

	virtual void Activate() override;

private:
	FInventorySlot InventorySlot;
};

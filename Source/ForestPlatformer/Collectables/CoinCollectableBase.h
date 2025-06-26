// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableBase.h"
#include "CoinCollectableBase.generated.h"

UCLASS()
class FORESTPLATFORMER_API ACoinCollectableBase : public ACollectableBase
{
	GENERATED_BODY()

protected:
	virtual void OnCollectableOverlapped_Implementation(AActor* InOverlappedActor) override;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int32 CoinsValue = 1;
};

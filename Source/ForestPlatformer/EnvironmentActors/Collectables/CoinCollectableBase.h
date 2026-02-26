// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CollectableBase.h"
#include "CoinCollectableBase.generated.h"

UCLASS()
class FORESTPLATFORMER_API ACoinCollectableBase : public ACollectableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnCollected(AActor* InInstigator) override;

	/** The number of coins this collectable contains */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int32 CoinsValue = 1;
};

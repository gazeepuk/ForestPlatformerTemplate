// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FPAnimInstanceBase.h"
#include "FPEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UFPEnemyAnimInstance : public UFPAnimInstanceBase
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};

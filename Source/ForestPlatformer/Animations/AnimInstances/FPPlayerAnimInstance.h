// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FPAnimInstanceBase.h"
#include "FPPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UFPPlayerAnimInstance : public UFPAnimInstanceBase
{
	GENERATED_BODY()
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};

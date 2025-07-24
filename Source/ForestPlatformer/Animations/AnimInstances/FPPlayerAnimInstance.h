// Fill out your copyright notice in the Description page of Project Settings.

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

// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEnemyAnimInstance.h"

void UFPEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	bShouldMove = GroundSpeed > 3.f && !bFalling;
}

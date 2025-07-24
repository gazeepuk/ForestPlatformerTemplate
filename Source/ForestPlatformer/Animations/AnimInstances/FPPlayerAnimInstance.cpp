// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UFPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(OwningMovementComponent)
	{
		bShouldMove = !bFalling && GroundSpeed > 3.f && !OwningMovementComponent->GetCurrentAcceleration().IsNearlyZero();
	}
}

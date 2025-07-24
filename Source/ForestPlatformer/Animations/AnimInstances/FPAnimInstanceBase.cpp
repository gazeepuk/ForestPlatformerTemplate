// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAnimInstanceBase.h"

#include "KismetAnimationLibrary.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFPAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ACharacter>(GetOwningActor());
	OwningMovementComponent = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
}

void UFPAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	Velocity = OwningCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());
	bFalling = OwningMovementComponent->IsFalling();
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}

bool UFPAnimInstanceBase::DoesOwnerHaveTag(FGameplayTag InGameplayTagToCheck) const
{
	return UFPFunctionLibrary::NativeDoesActorHaveTag(GetOwningActor(), InGameplayTagToCheck);
}

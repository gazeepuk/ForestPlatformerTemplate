// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacterMovementComponent.h"

void UFPCharacterMovementComponent::StartFloating_Implementation()
{
	if(!bFloating)
	{
		GravityScale *= FloatingGravityMultiplier;
		bFloating = true;
	}
}

void UFPCharacterMovementComponent::StopFloating_Implementation()
{
	if(bFloating)
	{
		GravityScale /= FloatingGravityMultiplier;
		bFloating = false;
	}
}

void UFPCharacterMovementComponent::HandleFloating_Implementation()
{
	if(GetOwner()->GetVelocity().Z < 0.f)
	{
		StartFloating();
	}
	else
	{
		StopFloating();
	}
}

float UFPCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed() * GetSpeedMultiplier();
}

void UFPCharacterMovementComponent::SetSpeedMultiplier(float InSpeedMultiplier)
{
	SpeedMultiplier = FMath::Max(0.f, InSpeedMultiplier);
}

float UFPCharacterMovementComponent::GetSpeedMultiplier() const
{
	return SpeedMultiplier;
}

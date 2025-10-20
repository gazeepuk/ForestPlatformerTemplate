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

void UFPCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	MaxWalkSpeed = GetCurrentMovementStateSpeed();
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

void UFPCharacterMovementComponent::SetCurrentMovementState(EFPMovementState InMovementState)
{
	if(CurrentMovementState == InMovementState)
	{
		return;
	}
	
	CurrentMovementState = InMovementState;
	HandleMovementState();
}

EFPMovementState UFPCharacterMovementComponent::GetCurrentMovementState() const
{
	return CurrentMovementState;
}

float UFPCharacterMovementComponent::GetMovementStateSpeed(EFPMovementState InMovementState) const
{
	switch (InMovementState)
	{
	case EFPMovementState::Normal:
		return NormalSpeed;
	case EFPMovementState::Strafe:
		return StrafeSpeed;
	default:
		return 0.f;
	}
}

float UFPCharacterMovementComponent::GetCurrentMovementStateSpeed() const
{
	return GetMovementStateSpeed(CurrentMovementState);
}

void UFPCharacterMovementComponent::HandleMovementState()
{
	switch (CurrentMovementState)
	{
	case EFPMovementState::Normal:
		bOrientRotationToMovement = true;
		break;
	case EFPMovementState::Strafe:
		bOrientRotationToMovement = false;
		break;
	default:
		break;
	}

	MaxWalkSpeed = GetCurrentMovementStateSpeed();
}

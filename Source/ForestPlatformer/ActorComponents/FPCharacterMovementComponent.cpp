// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacterMovementComponent.h"

void UFPCharacterMovementComponent::StartFloating()
{
	if(!bFloating)
	{
		GravityScale *= FloatingSpeedMultiplier;
		bFloating = true;
	}
}

void UFPCharacterMovementComponent::StopFloating()
{
	if(bFloating)
	{
		GravityScale /= FloatingSpeedMultiplier;
		bFloating = false;
	}
}

void UFPCharacterMovementComponent::HandleFloating()
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

// Developed by Ivan Piankouski. All Rights Reserved


#include "FPCharacterMovementComponent.h"

void UFPCharacterMovementComponent::StartFloating_Implementation()
{
	if(!bFloating)
	{
		bFloating = true;
		AddMovementModifier(EMovementModifier::GravityScale, FloatingGravityMultiplier);
	}
}

void UFPCharacterMovementComponent::StopFloating_Implementation()
{
	if(bFloating)
	{
		RemoveMovementModifier(EMovementModifier::GravityScale, FloatingGravityMultiplier);
		bFloating = false;
	}
}

void UFPCharacterMovementComponent::HandleFloating_Implementation()
{
	// Starts floating if the 
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
	// Sets max walk speed of the current movement state to ensure it's correct
	MaxWalkSpeed = GetCurrentMovementStateSpeed();
}

float UFPCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed() * GetMovementModifiers(EMovementModifier::MaxWalkSpeed);
}

float UFPCharacterMovementComponent::GetGravityZ() const
{
	return Super::GetGravityZ() * GetMovementModifiers(EMovementModifier::GravityScale);
}

void UFPCharacterMovementComponent::AddMovementModifier(EMovementModifier InMovementModifier, float InMultiplier)
{
	TArray<float>& Modifiers = MovementModifiers.FindOrAdd(InMovementModifier);
	Modifiers.Add(InMultiplier);
}

void UFPCharacterMovementComponent::RemoveMovementModifier(EMovementModifier InMovementModifier, float InMultiplier)
{
	if(TArray<float>* Modifiers = MovementModifiers.Find(InMovementModifier))
	{
		Modifiers->RemoveSingle(InMultiplier);
	}
}

float UFPCharacterMovementComponent::GetMovementModifiers(EMovementModifier InMovementModifier) const
{
	float FinalModifier = 1.f;
	if(const TArray<float>* Modifiers = MovementModifiers.Find(InMovementModifier))
	{
		if (Modifiers->IsEmpty())
		{
			return FinalModifier;
		}

		for (const float Modifier : *Modifiers)
		{
			FinalModifier *= Modifier;
		} 
	}

	return FinalModifier;
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

float UFPCharacterMovementComponent::GetMovementStateSpeed_Implementation(EFPMovementState InMovementState) const
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

void UFPCharacterMovementComponent::HandleMovementState_Implementation()
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

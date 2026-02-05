// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchingComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void ULaunchingComponent::LaunchInDirection(FVector InDirection, float InLaunchingForce)
{
	float FinalLaunchingForce = InLaunchingForce < 0.f ? DefaultLaunchForce : InLaunchingForce;
	LaunchWithParams(InDirection, FinalLaunchingForce, bResolveLaunchingDirection);
}

void ULaunchingComponent::LaunchFromImpactPoint(FVector InImpactPoint, float InLaunchingForce)
{
	FVector RawDirection = GetOwner()->GetActorLocation() - InImpactPoint;

	if(bUsePawnEyes)
	{
		if(APawn* PawnOwner = GetOwner<APawn>())
		{
			RawDirection = PawnOwner->GetPawnViewLocation() - InImpactPoint;
		}
	}

	float FinalLaunchingForce = InLaunchingForce < 0.f ? DefaultLaunchForce : InLaunchingForce;
	LaunchWithParams(RawDirection, FinalLaunchingForce, bResolveLaunchingDirection);
}

void ULaunchingComponent::LaunchWithParams(FVector InDirectionVector, float InLaunchStrength,
                                           bool bResolveDirection)
{
	if(bResolveDirection)
	{
		InDirectionVector = ResolveLaunchingDirection(InDirectionVector);
	}

	FVector LaunchingForce = InDirectionVector * InLaunchStrength;
	
	if(ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		OwnerCharacter->LaunchCharacter(LaunchingForce, true, true);
	}
}

FVector ULaunchingComponent::ResolveLaunchingDirection(const FVector& InRawDirection) const
{
	FVector ResolvedDirection = InRawDirection.GetSafeNormal();

	ResolvedDirection.Z = IsOwnerGrounded() ? 1.f : FMath::Sign(ResolvedDirection.Z);

	return ResolvedDirection.GetSafeNormal();
}

bool ULaunchingComponent::IsOwnerGrounded() const
{
	if(const ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		if (const UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			return CharacterMovementComponent->IsMovingOnGround();
		}
	}

	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSpringArmComponent.h"


UFPSpringArmComponent::UFPSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFPSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	FinalTargetArmLength = TargetArmLength;
	
	OwnerPawn = GetOwner<APawn>();
	if(OwnerPawn.IsValid())
	{
		OwnerController = OwnerPawn->GetController();
	}
}

void UFPSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bEnableMovementAlignment)
	{
		UpdateMovementAlignment(DeltaTime);
	}

	if(!FMath::IsNearlyEqual(TargetArmLength, FinalTargetArmLength))
	{
		const float Distance = FinalTargetArmLength - TargetArmLength;
		const float AbsDistance = FMath::Abs(Distance);
		if(AbsDistance < 1.f)
		{
			TargetArmLength = FinalTargetArmLength;
		}
		else
		{
			TargetArmLength = FMath::FInterpTo(TargetArmLength, FinalTargetArmLength, DeltaTime, ZoomingSpeed);
		}
	}
	else
	{
		TargetArmLength = FinalTargetArmLength;
	}
}

void UFPSpringArmComponent::AddTargetArmLength(float InAdditiveLength)
{
	float NewTargetArmLength = FinalTargetArmLength + InAdditiveLength;
	FinalTargetArmLength = bClampTargetArmLength ? FMath::Clamp(NewTargetArmLength, MinTargetArmLength, MaxTargetArmLength) : NewTargetArmLength;
}

void UFPSpringArmComponent::StopZooming()
{
	FinalTargetArmLength = TargetArmLength;
}

void UFPSpringArmComponent::ZoomMouse(float InZoomingMultiplier)
{
	AddTargetArmLength(MouseZoomingStep * InZoomingMultiplier);
}

void UFPSpringArmComponent::ZoomController(float InZoomingMultiplier)
{
	AddTargetArmLength(ControllerZoomingStep * InZoomingMultiplier);
}

void UFPSpringArmComponent::UpdateMovementAlignment(float InDeltaTime)
{
	if(!OwnerController.IsValid() || !OwnerPawn.IsValid())
	{
		return;
	}

	FVector OwnerVelocity = OwnerPawn->GetVelocity();
	OwnerVelocity.Z = 1.f;
	
	if(OwnerVelocity.SizeSquared() < FMath::Square(VelocityAlignmentThreshold))
	{
		return;
	}

	const FRotator CurrentControlRotation = OwnerController->GetControlRotation();
	const FRotator TargetControlRotation = OwnerVelocity.Rotation();

	const float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentControlRotation.Yaw, TargetControlRotation.Yaw);
	const float AbsDeltaYaw = FMath::Abs(DeltaYaw);
	
	if(AbsDeltaYaw > MovementAlignmentFrontAngleThreshold && AbsDeltaYaw < MovementAlignmentBackAngleThreshold)
	{
		const float InterpSpeed = FMath::GetMappedRangeValueClamped
		(
			FVector2D(MovementAlignmentFrontAngleThreshold, 180.f),
			FVector2D(0.f, MovementAlignmentSpeed),
			AbsDeltaYaw
		);
		
		const FRotator NewRotation = FMath::RInterpTo
		(
			CurrentControlRotation,
			TargetControlRotation,
			InDeltaTime,
			InterpSpeed
		);

		OwnerController->SetControlRotation(FRotator(CurrentControlRotation.Pitch, NewRotation.Yaw, CurrentControlRotation.Roll));
	}
}

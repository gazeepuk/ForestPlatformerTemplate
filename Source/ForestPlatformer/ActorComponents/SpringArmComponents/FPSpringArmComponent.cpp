// Developed by Ivan Piankouski. All Rights Reserved


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

	// Finds the distance between current and the final lengths 
	const float Distance = FinalTargetArmLength - TargetArmLength;
	const float AbsDistance = FMath::Abs(Distance);
	
	// Interpolates current target arm length to the final target length if the distance is big enough
	if(!(AbsDistance < SnappingDistance))
	{
		TargetArmLength = FMath::FInterpTo(TargetArmLength, FinalTargetArmLength, DeltaTime, ZoomingSpeed);
	}
	// Else snapping the current length to the final length
	else
	{
		TargetArmLength = FinalTargetArmLength;
	}
}

void UFPSpringArmComponent::AddTargetArmLength(float InAdditiveLength)
{
	// Sets new final target arm length
	float NewTargetArmLength = FinalTargetArmLength + InAdditiveLength;
	// Clamps the final target arm length
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
	// Checks controller and pawn pointers and tries to get them, if they are not valid
	if(!OwnerController.IsValid() || !OwnerPawn.IsValid())
	{
		OwnerPawn = GetOwner<APawn>();
		OwnerController = OwnerPawn.IsValid() ? OwnerPawn->GetController() : nullptr;

		if(!OwnerController.IsValid() || !OwnerPawn.IsValid())
		{
			return;
		}
	}

	FVector OwnerVelocity = OwnerPawn->GetVelocity();
	// Set 0 for Z velocity to prevent alignment, when the pawn is in air and does not move
	OwnerVelocity.Z = 0.f;
	
	if(OwnerVelocity.SizeSquared() < FMath::Square(VelocityAlignmentThreshold))
	{
		return;
	}

	const FRotator CurrentControlRotation = OwnerController->GetControlRotation();
	const FRotator TargetControlRotation = OwnerVelocity.Rotation();
	
	const float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentControlRotation.Yaw, TargetControlRotation.Yaw);
	const float AbsDeltaYaw = FMath::Abs(DeltaYaw);
	
	// Aligns controller rotation only the yaw difference between angle thresholds
	if(AbsDeltaYaw > MovementAlignmentFrontAngleThreshold && AbsDeltaYaw < MovementAlignmentBackAngleThreshold)
	{
		// Clamps alignment rotation speed. The greater the difference, the faster alignment
		const float InterpSpeed = FMath::GetMappedRangeValueClamped
		(
			FVector2D(MovementAlignmentFrontAngleThreshold, 180.f),
			FVector2D(0.f, MovementAlignmentSpeed),
			AbsDeltaYaw
		);

		// Interpolates rotation
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

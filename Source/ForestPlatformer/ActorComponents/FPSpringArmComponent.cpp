// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSpringArmComponent.h"


UFPSpringArmComponent::UFPSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFPSpringArmComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FinalTargetArmLength = TargetArmLength;
}

void UFPSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = GetOwner<APawn>();
	if(OwnerPawn.IsValid())
	{
		OwnerController = OwnerPawn->GetController();
	}
}

void UFPSpringArmComponent::SetTargetArmLengthLerp(float InNewTargetArmLength, float InLerpDuration)
{
	GetWorld()->GetTimerManager().ClearTimer(SetTargetArmLengthLerpHandle);

	FinalTargetArmLength = bClampTargetArmLength ? FMath::Clamp(InNewTargetArmLength, MinTargetArmLength, MaxTargetArmLength) : InNewTargetArmLength;
	TargetArmLengthLerpDuration = InLerpDuration;
	LerpStartTime = GetWorld()->GetTimeSeconds();
	
	GetWorld()->GetTimerManager().SetTimer(SetTargetArmLengthLerpHandle, this, &UFPSpringArmComponent::UpdateTargetArmLength, 0.02f, true);
}

void UFPSpringArmComponent::AddTargetArmLength(float InZoomMultiplier)
{
	SetTargetArmLengthLerp(FinalTargetArmLength + ZoomingStep * InZoomMultiplier, TargetArmLengthLerpDuration);
}

void UFPSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bEnableMovementAlignment)
	{
		UpdateMovementAlignment(DeltaTime);
	}
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

void UFPSpringArmComponent::UpdateTargetArmLength()
{
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - LerpStartTime;
	const float LerpAlpha = FMath::Clamp(ElapsedTime / TargetArmLengthLerpDuration, 0.f, 1.f);
	
	TargetArmLength = FMath::Lerp(TargetArmLength, FinalTargetArmLength, LerpAlpha);

	if(FMath::IsNearlyEqual(TargetArmLength, FinalTargetArmLength) || LerpAlpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SetTargetArmLengthLerpHandle);
	}
}

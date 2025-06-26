// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSpringArmComponent.h"


void UFPSpringArmComponent::SetTargetArmLengthLerp(float InNewTargetArmLength, float InLerpDuration)
{
	GetWorld()->GetTimerManager().ClearTimer(SetTargetArmLengthLerpHandle);

	FinalTargetArmLength = bClampTargetArmLength ? FMath::Clamp(InNewTargetArmLength, MinTargetArmLength, MaxTargetArmLength) : InNewTargetArmLength;
	TargetArmLengthLerpDuration = InLerpDuration;
	LerpStartTime = GetWorld()->GetTimeSeconds();
	
	GetWorld()->GetTimerManager().SetTimer(SetTargetArmLengthLerpHandle, this, &UFPSpringArmComponent::UpdateTargetArmLength, 0.02f, true);
}

void UFPSpringArmComponent::AddTargetArmLength(float InAdditiveValue)
{
	SetTargetArmLengthLerp(FinalTargetArmLength + InAdditiveValue, TargetArmLengthLerpDuration);
}

void UFPSpringArmComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FinalTargetArmLength = TargetArmLength;
}

void UFPSpringArmComponent::UpdateTargetArmLength()
{
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - LerpStartTime;
	const float LerpAlpha = FMath::Clamp(ElapsedTime / TargetArmLengthLerpDuration, 0.f, 1.f);
	
	TargetArmLength = FMath::Lerp(TargetArmLength, FinalTargetArmLength, LerpAlpha);

	if(FMath::IsNearlyEqual(TargetArmLength, FinalTargetArmLength))
	{
		GetWorld()->GetTimerManager().ClearTimer(SetTargetArmLengthLerpHandle);
	}
}

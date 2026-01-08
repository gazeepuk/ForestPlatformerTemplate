// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneRotatingMovementComponent.h"


// Sets default values for this component's properties
USceneRotatingMovementComponent::USceneRotatingMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void USceneRotatingMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!TargetComponent)
	{
		TargetComponent = GetOwner()->GetRootComponent();
	}
}


// Called every frame
void USceneRotatingMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!TargetComponent || !bRotationEnabled)
	{
		return;
	}
	
	const FQuat DeltaQuat = FQuat(RotationAxis.GetSafeNormal(), FMath::DegreesToRadians(RotationSpeed * DeltaTime));

	TargetComponent->AddLocalRotation(DeltaQuat);
}


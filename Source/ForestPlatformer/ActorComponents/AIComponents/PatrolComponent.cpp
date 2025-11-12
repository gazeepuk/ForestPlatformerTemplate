// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolComponent.h"

#include "NavigationSystem.h"


bool UPatrolComponent::IsCurrentPatrolIndexValid() const
{
	// Returns true, if the current index is within bounds and points to a valid actor
	return CurrentPatrolPointIndex >= 0 && CurrentPatrolPointIndex < PatrolPoints.Num() && PatrolPoints[CurrentPatrolPointIndex] != nullptr && !PatrolPoints.IsEmpty();
}

FVector UPatrolComponent::GetPatrolPointLocationByIndex(int32 InPatrolPointIndex) const
{
	if(InPatrolPointIndex >= 0 && PatrolPoints.Num() > InPatrolPointIndex)
	{
		if(const AActor* PatrolPoint = PatrolPoints[InPatrolPointIndex])
		{
			return PatrolPoint->GetActorLocation();
		}
	}

	return FVector::ZeroVector;
}

FVector UPatrolComponent::GetCurrentPatrolPointLocation() const
{
	return GetPatrolPointLocationByIndex(CurrentPatrolPointIndex);
}

FVector UPatrolComponent::GetRandomLocationWithinPatrolBorders() const
{
	if(!PatrolBorders)
	{
		return FVector::ZeroVector;
	}

	FVector PatrolOrigin = PatrolBorders->GetActorLocation();
	FVector PatrolExtent = PatrolBorders->GetBounds().BoxExtent;

	// Uses navigation system to find a random point within patrol bounds. Returns zero vector, if the navigation or found location is invalid. 
	if(UNavigationSystemV1* NavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation RandomLocation;
		bool bFoundLocation = NavigationSystemV1->GetRandomReachablePointInRadius(PatrolOrigin, PatrolExtent.GetMax(), RandomLocation);

		return bFoundLocation ? RandomLocation.Location : FVector::ZeroVector;
	}
	
	return FVector::ZeroVector;
}

void UPatrolComponent::SetNextPatrolPointIndex()
{
	if(PatrolPoints.IsEmpty())
	{
		CurrentPatrolPointIndex = -1;
		return;
	}
	// Determines patrol direction. If moving forward, the index increments. If moving backward, the index decrements.
	const int32 PatrolDirection = bPatrolForward ? 1 : -1;
	// Clamps the index within the array bounds
	CurrentPatrolPointIndex = (CurrentPatrolPointIndex + PatrolDirection + PatrolPoints.Num()) % PatrolPoints.Num();
}

void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	// Sets the last patrol point as the current point so the first call of SetNextPatrolPointIndex() selects the first point.
	CurrentPatrolPointIndex = PatrolPoints.Num() - 1;
}

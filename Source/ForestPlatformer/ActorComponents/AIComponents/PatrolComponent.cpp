// Developed by Ivan Piankouski. All Rights Reserved


#include "PatrolComponent.h"

#include "NavigationSystem.h"

bool UPatrolComponent::FindNextPatrolLocation_Implementation(FVector& OutFoundLocation)
{
	OutFoundLocation = FVector::ZeroVector;

	switch (PatrolType)
	{
	case EPatrolType::Sequence:
		return FindNextPatrolLocationInSequence(OutFoundLocation);
	case EPatrolType::InBounds:
		return FindNextPatrolLocationInBounds(OutFoundLocation);
	default:
		break;
	}

	return false;
}

bool UPatrolComponent::IsCurrentPatrolIndexValid() const
{
	// Returns true, if the current index is within bounds and points to a valid actor
	return CurrentPatrolPointIndex >= 0 && CurrentPatrolPointIndex < PatrolPoints.Num() && PatrolPoints[CurrentPatrolPointIndex] != nullptr && !PatrolPoints.IsEmpty();
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

FVector UPatrolComponent::GetCurrentPatrolPointLocation() const
{
	return GetPatrolPointLocationByIndex(CurrentPatrolPointIndex);
}

FVector UPatrolComponent::GetPatrolPointLocationByIndex(int32 InPatrolPointIndex) const
{
	if(PatrolPoints.IsValidIndex(InPatrolPointIndex))
	{
		if(const AActor* PatrolPoint = PatrolPoints[InPatrolPointIndex])
		{
			return PatrolPoint->GetActorLocation();
		}
	}

	return FVector::ZeroVector;
}

void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	// Sets the last patrol point as the current point so the first call of SetNextPatrolPointIndex() selects the first point.
	CurrentPatrolPointIndex = PatrolPoints.Num() - 1;
}

bool UPatrolComponent::FindNextPatrolLocationInSequence(FVector& OutFoundLocation)
{
	SetNextPatrolPointIndex();
	
	if(!IsCurrentPatrolIndexValid())
	{
		return false;
	}

	OutFoundLocation = GetCurrentPatrolPointLocation();
	
	return true;
}

bool UPatrolComponent::FindNextPatrolLocationInBounds(FVector& OutFoundLocation) const
{
	if(!PatrolBorders)
	{
		return false;
	}

	FVector PatrolOrigin = PatrolBorders->GetActorLocation();
	FVector PatrolExtent = PatrolBorders->GetBounds().BoxExtent;

	// Uses navigation system to find a random point within patrol bounds. Returns zero vector, if the navigation or found location is invalid. 
	if(UNavigationSystemV1* NavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation RandomLocation;
		bool bFoundLocation = NavigationSystemV1->GetRandomReachablePointInRadius(PatrolOrigin, PatrolExtent.GetMax(), RandomLocation);

		OutFoundLocation = bFoundLocation ? RandomLocation.Location : FVector::ZeroVector;
		return bFoundLocation;
	}
	
	return false;
}

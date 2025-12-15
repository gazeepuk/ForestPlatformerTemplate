// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolDataTypes.h"

#include "ActorComponents/AIComponents/PatrolComponent.h"


bool UPatrolDataBase::GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const
{
	return false;
}

bool UPatrolInBoundsData::GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const
{
	if(!PatrollingPawn)
	{
		return false;
	}

	UPatrolComponent* PatrolComponent = PatrollingPawn->GetComponentByClass<UPatrolComponent>();
	if(!PatrollingPawn)
	{
		return false;
	}

	if(!PatrolComponent->IsPatrolBordersValid())
	{
		return false;
	}

	OutNextLocation = PatrolComponent->GetRandomLocationWithinPatrolBorders();
	return true;
}

bool UPatrolSequence::GetNextPatrolLocation_Implementation(APawn* PatrollingPawn, FVector& OutNextLocation) const
{
 	if(!PatrollingPawn)
	{
		return false;
	}

	UPatrolComponent* PatrolComponent = PatrollingPawn->GetComponentByClass<UPatrolComponent>();
	if(!PatrolComponent)
	{
		return false;
	}

	PatrolComponent->SetNextPatrolPointIndex();
	
	if(!PatrolComponent->IsCurrentPatrolIndexValid())
	{
		return false;
	}

	OutNextLocation = PatrolComponent->GetCurrentPatrolPointLocation();
	return true;
}

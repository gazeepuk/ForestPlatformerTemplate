// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/FPAsyncLaunchActor.h"

UFPAsyncLaunchActor* UFPAsyncLaunchActor::LaunchActor(UObject* WorldContextObject, AActor* InLaunchingActor,
	FVector LaunchDirection, float Distance, float MaxHeight, float Duration, bool bOrientToMovement)
{
	if(!InLaunchingActor)
	{
		return nullptr;
	}
	
	UFPAsyncLaunchActor* Task = NewObject<UFPAsyncLaunchActor>();
	Task->WorldContext = WorldContextObject;
	Task->LaunchingActor = InLaunchingActor;
	Task->StartLocation = InLaunchingActor->GetActorLocation();
	Task->TargetLocation = Task->StartLocation + LaunchDirection.GetSafeNormal() * Distance;
	Task->ControlPoint = (Task->StartLocation + Task->TargetLocation) * 0.5f + FVector(0,0,MaxHeight);
	Task->TotalDuration = FMath::Max(0, Duration);
	Task->bShouldOrientToMovement = bOrientToMovement;
	Task->RegisterWithGameInstance(WorldContextObject);

	return Task;
}

void UFPAsyncLaunchActor::Activate()
{
	if(!LaunchingActor->IsValidLowLevel() || !GetWorld())
	{
		Cleanup();
		return;
	}

	ElapsedTime = 0.f;
	LaunchTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFPAsyncLaunchActor::TickMovement);
}

void UFPAsyncLaunchActor::TickMovement()
{
	if(!LaunchingActor->IsValidLowLevel() || !GetWorld())
	{
		Cleanup();
		return;
	}

	ElapsedTime += GetWorld()->GetDeltaSeconds();
	const float Alpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f);
	const FVector NewLocation = FMath::Lerp(
		FMath::Lerp(StartLocation, ControlPoint, Alpha),
		FMath::Lerp(ControlPoint, TargetLocation, Alpha),
		Alpha);
	
	if(bShouldOrientToMovement && Alpha < 1.f)
	{
		const FVector Direction = NewLocation - LaunchingActor->GetActorLocation();
		if(Direction.SizeSquared() > SMALL_NUMBER)
		{
			LaunchingActor->SetActorRotation(Direction.Rotation());
		}
	}
	
	LaunchingActor->SetActorLocation(NewLocation, true);

	if(Alpha < 1.f && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LaunchTimerHandle);
		LaunchTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFPAsyncLaunchActor::TickMovement);
	}
	else
	{
		Cleanup();
	}
}

void UFPAsyncLaunchActor::Cleanup()
{
	OnLaunchCompleted.Broadcast();
	SetReadyToDestroy();
}

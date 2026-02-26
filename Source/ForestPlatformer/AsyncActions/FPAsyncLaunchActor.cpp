// Developed by Ivan Piankouski. All Rights Reserved


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
	Task->bFalling = false;
	Task->CurrentVelocity = FVector::ZeroVector;
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

	if(!bFalling)
	{
		ParabolicMovement();
	}
	else
	{
		FallingMovement();
	}
}

void UFPAsyncLaunchActor::Cleanup()
{
	OnLaunchCompleted.Broadcast();
	SetReadyToDestroy();
}

void UFPAsyncLaunchActor::ParabolicMovement()
{
	const float Alpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f);
	// Finds new location on a parabola
	const FVector NewLocation = FMath::Lerp(
		FMath::Lerp(StartLocation, ControlPoint, Alpha),
		FMath::Lerp(ControlPoint, TargetLocation, Alpha),
		Alpha);

	if(Alpha >= 1.f)
	{
		SwitchToFalling();
		return;
	}

	// Finds velocity based on new and previous locations
	CurrentVelocity = (NewLocation - LaunchingActor->GetActorLocation()) / GetWorld()->GetDeltaSeconds();
	
	if(bShouldOrientToMovement)
	{
		const FVector Direction = NewLocation - LaunchingActor->GetActorLocation();
		if(Direction.SizeSquared() > SMALL_NUMBER)
		{
			LaunchingActor->SetActorRotation(Direction.Rotation());
		}
	}
	
	LaunchingActor->SetActorLocation(NewLocation, true);

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LaunchTimerHandle);
		LaunchTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFPAsyncLaunchActor::TickMovement);
	}
}

void UFPAsyncLaunchActor::FallingMovement()
{
	// Adds gravity force to the current velocity
	CurrentVelocity.Z += GetWorld()->GetGravityZ() * GetWorld()->GetDeltaSeconds();
	
	FVector CurrentLocation = LaunchingActor->GetActorLocation();
	FVector NewLocation = CurrentLocation + CurrentVelocity * GetWorld()->GetDeltaSeconds();
	
	FHitResult HitResult;
	FVector TraceStart = CurrentLocation;
	FVector TraceEnd = NewLocation;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(LaunchingActor.Get());

	// Snaps the actor to the ground, if it's too close to the ground
	if(GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic, CollisionParams))
	{
		LaunchingActor->SetActorLocation(HitResult.Location, true);
		Cleanup();
		return;
	}
	
	LaunchingActor->SetActorLocation(NewLocation, true);

	if(bShouldOrientToMovement && CurrentVelocity.SizeSquared() > SMALL_NUMBER)
	{
		LaunchingActor->SetActorRotation(CurrentVelocity.Rotation());
	}
 
	if(GetWorld())
	{
		LaunchTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFPAsyncLaunchActor::TickMovement);
	}
}

void UFPAsyncLaunchActor::SwitchToFalling()
{
	bFalling = true;
	CurrentVelocity.Z = -FMath::Abs(CurrentVelocity.Z) * 0.75f;

	if(GetWorld())
	{
		LaunchTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFPAsyncLaunchActor::TickMovement);
	}
}

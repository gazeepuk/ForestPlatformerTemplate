// Developed by Ivan Piankouski. All Rights Reserved


#include "WindBoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UWindBoxComponent::UWindBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxBeginOverlap);
	OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxEndOverlap);
}

void UWindBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bWindActive)
	{
		const FVector WindDirection = GetUpVector().GetSafeNormal();
		const FVector WindForce = WindDirection * WindStrength;

		// Tries to lift all overlapped actor
		for (AActor* LiftingActor : OverlappedActors)
		{
			if(!IsValid(LiftingActor))
			{
				continue;
			}

			// Lifts only character actors
			if(const ACharacter* LiftingCharacter = Cast<ACharacter>(LiftingActor))
			{
				if(UCharacterMovementComponent* CharacterMovementComponent = LiftingCharacter->GetCharacterMovement())
				{
					// Calculates antigravity force to cancel gravity 
					const FVector AntiGravityForce = CharacterMovementComponent->GetGravityDirection() * CharacterMovementComponent->GravityScale * CharacterMovementComponent->GetGravityZ();
					// Calculates dump force to cancel character velocity
					const FVector DumpingForce = -CharacterMovementComponent->Velocity;
					// Calculates final force to apply
					const FVector ForceToApply = (AntiGravityForce + DumpingForce + WindForce) * DeltaTime;
					
					CharacterMovementComponent->Velocity += ForceToApply;
				}
			}
		} 
	}
}

void UWindBoxComponent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		OverlappedActors.AddUnique(OtherActor);
	}

	// Slightly upward launch character to disable walking mode
	if(ACharacter* LiftingCharacter = Cast<ACharacter>(OtherActor))
	{
		const FVector UpwardLaunch = FVector(0.f, 0.f, 10.f);
		LiftingCharacter->LaunchCharacter(UpwardLaunch, false, true);
	}

	// Disable tick function, if there is no lifting actors
	if(!OverlappedActors.IsEmpty() && !PrimaryComponentTick.IsTickFunctionEnabled())
	{
		PrimaryComponentTick.SetTickFunctionEnable(true);
	}
}

void UWindBoxComponent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		OverlappedActors.Remove(OtherActor);
	}

	// Disable tick function, if there is no lifting actors
	if (OverlappedActors.IsEmpty() && PrimaryComponentTick.IsTickFunctionEnabled())
	{
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


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

		for (AActor* LiftingActor : OverlappedActors)
		{
			if(!IsValid(LiftingActor))
			{
				continue;
			}

			if(const ACharacter* LiftingCharacter = Cast<ACharacter>(LiftingActor))
			{
				if(UCharacterMovementComponent* CharacterMovementComponent = LiftingCharacter->GetCharacterMovement())
				{
					const FVector AntiGravityForce = CharacterMovementComponent->GetGravityDirection() * CharacterMovementComponent->GravityScale * CharacterMovementComponent->GetGravityZ();
					const FVector DumpingForce = -CharacterMovementComponent->Velocity;
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

	if(ACharacter* LiftingCharacter = Cast<ACharacter>(OtherActor))
	{
		const FVector UpwardLaunch = FVector(0.f, 0.f, 10.f);
		LiftingCharacter->LaunchCharacter(UpwardLaunch, false, true);
	}
	
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

	if (OverlappedActors.IsEmpty() && PrimaryComponentTick.IsTickFunctionEnabled())
	{
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}



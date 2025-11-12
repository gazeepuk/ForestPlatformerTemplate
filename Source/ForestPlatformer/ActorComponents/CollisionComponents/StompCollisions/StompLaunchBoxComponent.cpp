// Fill out your copyright notice in the Description page of Project Settings.


#include "StompLaunchBoxComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UStompLaunchBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && IsActorAbove(OtherActor))
	{
		OnStompBoxOverlappedActorAbove.Broadcast(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		
		const FVector LaunchVelocity = GetUpVector() * LaunchActorForce;
		// Tries to launch the character or add impulse to root component
		if(ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor))
		{
			OtherCharacter->GetCharacterMovement()->Launch(LaunchVelocity);
		}
		else
		{
			UPrimitiveComponent* OtherRoot = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if(OtherRoot && OtherRoot->IsSimulatingPhysics())
			{
				OtherRoot->AddImpulse(LaunchVelocity, NAME_None, true);
			}
		}
	}
}

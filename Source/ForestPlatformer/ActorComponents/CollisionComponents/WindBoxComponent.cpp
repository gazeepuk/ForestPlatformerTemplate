// Fill out your copyright notice in the Description page of Project Settings.


#include "WindBoxComponent.h"

#include "Components/ArrowComponent.h"
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

	if(bLifting)
	{
		for (AActor* LiftingActor : OverlappedActors)
		{
			if(!LiftingActor)
			{
				continue;
			}

			if(ACharacter* LiftingCharacter = Cast<ACharacter>(LiftingActor))
			{
				const FVector AntiGravityForce = LiftingCharacter->GetCharacterMovement()->GetGravityDirection() * GetWorld()->GetGravityZ() * LiftingCharacter->GetCharacterMovement()->GravityScale * LiftingCharacter->GetCharacterMovement()->Mass;
				const FVector WindForce = GetUpVector() * LiftSpeed;
				LiftingCharacter->GetCharacterMovement()->AddForce(AntiGravityForce + WindForce);
			}
			else if(UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(LiftingActor->GetRootComponent()))
			{
				if(IsSimulatingPhysics())
				{
					const FVector WindForce = GetUpVector() * LiftSpeed;
					RootPrim->AddForce(WindForce);
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
		if(UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()))
		{
			RootPrimitive->SetEnableGravity(false);
			FVector RootVelocity = RootPrimitive->GetComponentVelocity();
			RootVelocity.Z =  FMath::Clamp(RootVelocity.Z, -125.f, 15.f);
			RootPrimitive->SetPhysicsLinearVelocity(RootVelocity);
		}
		if(ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor))
		{
			FVector CharacterVelocity = OtherCharacter->GetMovementComponent()->Velocity;
			CharacterVelocity.Z = FMath::Clamp(CharacterVelocity.Z, -200.f, 15.f);
			OtherCharacter->GetMovementComponent()->Velocity = CharacterVelocity;
		}
	}

	if(!OverlappedActors.IsEmpty() && !PrimaryComponentTick.IsTickFunctionEnabled())
	{
		PrimaryComponentTick.SetTickFunctionEnable(true);
	}
}

void UWindBoxComponent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->GetRootComponent()->Mobility == EComponentMobility::Type::Movable)
	{
		OverlappedActors.Remove(OtherActor);
		if(UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()))
		{
			RootPrimitive->SetEnableGravity(true);
		}
	}

	if (OverlappedActors.IsEmpty() && PrimaryComponentTick.IsTickFunctionEnabled())
	{
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}



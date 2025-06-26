// Fill out your copyright notice in the Description page of Project Settings.


#include "StompDamageBoxCollision.h"

#include "Characters/FPPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


UStompDamageBoxCollision::UStompDamageBoxCollision()
{
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UStompDamageBoxCollision::BeginPlay()
{
	Super::BeginPlay();
}

void UStompDamageBoxCollision::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || OtherActor == GetOwner())
	{
		return;
	}
	
	AFPPlayerCharacter* PlayerCharacter = Cast<AFPPlayerCharacter>(OtherActor);
	if(PlayerCharacter && IsActorAbove(PlayerCharacter))
	{
		OnStompBoxOverlappedActorAbove.Broadcast(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

		if(GetOwner()->Implements<UDamageableInterface>())
		{
			IDamageableInterface::Execute_TakeDamage(GetOwner(), PlayerCharacter, 1.f, PlayerCharacter->GetController());
		}
		
		PlayerCharacter->GetCharacterMovement()->Launch(GetComponentRotation().RotateVector(FVector::UpVector) * LaunchActorForce);
	}
}

bool UStompDamageBoxCollision::IsActorAbove(const AActor* InActor)
{
	if(const AFPPlayerCharacter* PlayerCharacter = Cast<AFPPlayerCharacter>(InActor))
	{
		FVector LocalPlayerLocation = GetComponentTransform().InverseTransformPosition(PlayerCharacter->GetActorLocation());
		
		return LocalPlayerLocation.Z >= StompHeightBuffer;
	}

	return false;
}

// Developed by Ivan Piankouski. All Rights Reserved


#include "StompDamageBoxCollision.h"

#include "Characters/FPPlayerCharacter.h"
#include "CoreTypes/CustomCollisions/FPCustomCollisions.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


UStompDamageBoxCollision::UStompDamageBoxCollision()
{
	// Makes collision detects only player
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);
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

		// Apply damage to owner if it implements IDamageableInterface
		if(GetOwner()->Implements<UDamageableInterface>())
		{
			IDamageableInterface::Execute_TakeDamage(GetOwner(), PlayerCharacter, DamageAmount, PlayerCharacter->GetController());
		}

		// Launches actor after stomping
		PlayerCharacter->GetCharacterMovement()->Launch(GetUpVector() * LaunchActorForce);
	}
}

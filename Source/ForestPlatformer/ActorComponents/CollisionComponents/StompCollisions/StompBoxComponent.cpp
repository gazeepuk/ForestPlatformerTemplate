// Developed by Ivan Piankouski. All Rights Reserved


#include "StompBoxComponent.h"

#include "CoreTypes/CustomCollisions/FPCustomCollisions.h"


UStompBoxComponent::UStompBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Makes collision detects only player and world dynamic objects
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SetCollisionResponseToChannel(ECC_FP_Player_OC, ECR_Overlap);
	
	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}


bool UStompBoxComponent::IsActorAbove(const AActor* InActor)
{
	if(!InActor || InActor == GetOwner())
	{
		return false;
	}

	// Finds overlapping actors location relative to this component world location
	FVector LocalActorLocation = GetComponentTransform().InverseTransformPosition(InActor->GetActorLocation());
	
	return LocalActorLocation.Z >= StompHeightBuffer;
}

void UStompBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsActorAbove(OtherActor))
	{
		OnStompBoxOverlappedActorAbove.Broadcast(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

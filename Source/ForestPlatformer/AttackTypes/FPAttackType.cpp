// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAttackType.h"

#include "GameFramework/Character.h"

bool UFPAttackType::CanAttack_Implementation()
{
	return true;
}

void UFPAttackType::PerformAttack_Implementation(AActor* InTargetActor)
{	
	UE_LOG(LogTemp, Display, TEXT("Performing Attack %s"), *AttackTypeID.ToString());
}

UWorld* UFPAttackType::GetWorld() const
{
	return OwningActor ? OwningActor->GetWorld() : nullptr;
}

ACharacter* UFPAttackType::GetCharacterFromOwningActor() const
{
	return Cast<ACharacter>(OwningActor);
}

USkeletalMeshComponent* UFPAttackType::GetSkeletalMeshFromOwningActor() const
{
	return OwningActor ? OwningActor->GetComponentByClass<USkeletalMeshComponent>() : nullptr;
}

void UFPAttackType::InitAttack(AActor* InOwningActor, UCombatComponentBase* InCombatComponent)
{
	SetOwningActor(InOwningActor);
	SetCombatComponent(InCombatComponent);
}

void UFPAttackType::EndAttack_Implementation()
{
	OnAttackEnded.Broadcast();
}

void UFPAttackType::PostInitProperties()
{
	UObject::PostInitProperties();

	AttackTypeIDHash = GetTypeHash(AttackTypeID);
    
	// Ensure default ID if empty
#if WITH_EDITOR
	if (AttackTypeID.IsNone())
	{
		AttackTypeID = *ThisClass::StaticClass()->GetName();
		AttackTypeIDHash = GetTypeHash(AttackTypeID);
	}
#endif
}

TArray<AActor*> UFPAttackType::FindTarget_Implementation()
{
	return TArray<AActor*>();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAttackType.h"

bool UFPAttackType::CanAttack_Implementation(AActor* InInstigator)
{
	return InInstigator != nullptr;
}

void UFPAttackType::PerformAttack_Implementation(AActor* InTargetActor)
{
	if(!TryPlayAttackAnimation())
	{
		UE_LOG(LogTemp, Error, TEXT("%s OnAttackMontageEndDelegate is bound? %hs"), *GetAttackTypeID().ToString(), OnAttackMontageEndDelegate.IsBound() ? "true" : "false");
		OnAttackMontageEndDelegate.ExecuteIfBound(nullptr, true);
	}
	
	UE_LOG(LogTemp, Display, TEXT("Performing Attack %s"), *AttackTypeID.ToString());
}

UWorld* UFPAttackType::GetWorld() const
{
	return OwningActor ? OwningActor->GetWorld() : nullptr;
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


bool UFPAttackType::TryPlayAttackAnimation()
{
	if(!AttackMontage || !OwningActor)
	{
		return false;
	}

	USkeletalMeshComponent* SkeletalMesh = OwningActor->GetComponentByClass<USkeletalMeshComponent>();
	if(!SkeletalMesh)
	{
		return false;
	}

	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if(!AnimInstance)
	{
		return false;
	}

	AnimInstance->Montage_Play(AttackMontage);
	
	AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndDelegate, AttackMontage);

	return true;
}


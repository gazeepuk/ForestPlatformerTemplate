// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAttackType.h"

#include "GameFramework/Character.h"

bool UFPAttackType::CanAttack_Implementation()
{
	return !IsOnCooldown();
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

void UFPAttackType::PerformAttack()
{
	if(bActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already active for %s! Canceling activation."), *AttackTypeID.ToString(), *GetNameSafe(GetOwningActor()));
		return;
	}

	bActive = true;
	StartCooldown();
	
	PerformAttackInner();
}

void UFPAttackType::PerformAttackInner_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Performing Attack %s"), *AttackTypeID.ToString());
}

void UFPAttackType::EndAttack()
{
	if(IsActive())
	{
		bActive = false;
		StopAttackMontage();
		EndAttackInner();
		OnAttackEnded.Broadcast();
	}
}

bool UFPAttackType::TryAbortAttack()
{
	if(!bInterruptible)
	{
		EndAttack();
		return true;
	}

	return false;
}

void UFPAttackType::EndAttackInner_Implementation()
{
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

void UFPAttackType::StopAttackMontage(float BlendOut)
{
	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMeshFromOwningActor();
	if(!SkeletalMesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if(!AnimInstance)
	{
		return;
	}

	if(AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Stop(BlendOut, AttackMontage);
	}
}

void UFPAttackType::StartCooldown()
{
	UWorld* World = GetWorld();
	if(!World)
	{
		return;
	}

	ResetCooldown();
	
	if(AttackCooldown > 0.f)
	{
		RemainingCooldown = AttackCooldown;
		World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ThisClass::UpdateCooldown, CooldownTick, true);
	}
}

void UFPAttackType::ResetCooldown()
{
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
	RemainingCooldown = 0.f;
}

void UFPAttackType::UpdateCooldown()
{
	if(!GetWorld())
	{
		ResetCooldown();
		return;
	}

	RemainingCooldown -= CooldownTick;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "piss");	
	if(RemainingCooldown <= 0.f)
	{
		ResetCooldown();
	}
}

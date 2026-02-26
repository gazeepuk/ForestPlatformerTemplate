// Developed by Ivan Piankouski. All Rights Reserved


#include "FPAttackType.h"

#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogFpAttackType);

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
		UE_LOG(LogFpAttackType, Warning, TEXT("%s is already active for %s! Canceling activation."), *GetAttackTypeID().ToString(), *GetNameSafe(GetOwningActor()));
		return;
	}

	bActive = true;
	StartCooldown();
	
	PerformAttackInner();
}

void UFPAttackType::PerformAttackInner_Implementation()
{
	UE_LOG(LogFpAttackType, Display, TEXT("Performing Attack %s"), *GetAttackTypeID().ToString());
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

	ClearCooldown();
	
	if(AttackCooldown > 0.f)
	{
		RemainingCooldown = AttackCooldown;
		World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ThisClass::UpdateCooldown, CooldownTick, true);
	}
}

void UFPAttackType::ClearCooldown()
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
		ClearCooldown();
		return;
	}

	RemainingCooldown -= CooldownTick;
	if(RemainingCooldown <= 0.f)
	{
		ClearCooldown();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponentBase.h"

#include "CoreTypes/AttackTypes/FPAttackType.h"
#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"



bool UCombatComponentBase::CanAttack_Implementation()
{
	return !UFPFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
}

bool UCombatComponentBase::TryActivateAttackByTag_Implementation(const FGameplayTag& InAttackTypeTag)
{
	if(UFPAttackType* AttackType = FindAttackTypeByTag(InAttackTypeTag))
	{
		return TryActivateAttack(AttackType);
	}
	
	return false;
}

void UCombatComponentBase::InitAttacks()
{
	for (const TSubclassOf<UFPAttackType>& DefaultAttackClass : DefaultAttackTypeClasses)
	{
		GrantAttackTypeByClass(DefaultAttackClass);
	}
}

UFPAttackType* UCombatComponentBase::GrantAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass)
{
	if(!InAttackTypeClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add an attack type for %s! The class is invalid!"), *GetNameSafe(GetOwner()));
		return nullptr;
	}

	if(UFPAttackType* AttackType = NewObject<UFPAttackType>(this, InAttackTypeClass))
	{
		AvailableAttackTypes.AddUnique(AttackType);
		UE_LOG(LogTemp, Display, TEXT("Added %s to %s attacks"), *AttackType->GetAttackTypeID().ToString(), *GetNameSafe(GetOwner()));

		AttackType->InitAttack(GetOwner(), this);
		AttackType->OnAttackEnded.AddUniqueDynamic(this, &ThisClass::OnAttackEnded);
		return AttackType;
	}
	return nullptr;
}

void UCombatComponentBase::RemoveAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass)
{

	if(UFPAttackType* FoundAttackType = FindAttackTypeByClass(InAttackTypeClass))
	{
		AvailableAttackTypes.Remove(FoundAttackType);
		if(FoundAttackType->IsActive())
		{
			FoundAttackType->EndAttack(); 
		}
		FoundAttackType->OnAttackEnded.RemoveDynamic(this, &ThisClass::OnAttackEnded);
		FoundAttackType->ConditionalBeginDestroy();
	}
}

UFPAttackType* UCombatComponentBase::FindAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass)
{
	
	if(!InAttackTypeClass)
	{
		return nullptr;
	}
	
	UFPAttackType** FoundAttack = AvailableAttackTypes.FindByPredicate([InAttackTypeClass](const UFPAttackType* AttackType)
	{
		return AttackType && AttackType->GetClass() == InAttackTypeClass;
	});
	
	return FoundAttack ? *FoundAttack : nullptr;
}

UFPAttackType* UCombatComponentBase::FindAttackTypeByTag(const FGameplayTag& InAttackTypeTag)
{
	if(!InAttackTypeTag.IsValid())
	{
		return nullptr;
	}
	
	UFPAttackType** FoundedAttackType = AvailableAttackTypes.FindByPredicate([InAttackTypeTag](const UFPAttackType* AttackType)
	{
		return AttackType && AttackType->GetAttackTypeTag().MatchesTagExact(InAttackTypeTag);
	});

	return FoundedAttackType ? *FoundedAttackType : nullptr;
}

bool UCombatComponentBase::TryAbortActiveAttack()
{
	if(ActiveAttack && ActiveAttack->IsActive())
	{
		return ActiveAttack->TryAbortAttack();
	}

	return true;
}

bool UCombatComponentBase::CanActivateAttackByTag(FGameplayTag InAttackTypeTag)
{
	if(UFPAttackType* AttackType = FindAttackTypeByTag(InAttackTypeTag))
	{
		return AttackType->CanAttack();
	}
	return false;
}

float UCombatComponentBase::GetAttackRemainingCooldown(FGameplayTag InAttackTypeTag)
{
	if(UFPAttackType* AttackType = FindAttackTypeByTag(InAttackTypeTag))
	{
		return AttackType->GetRemainingCooldownTime();
	}
	return -1.f;
}

void UCombatComponentBase::OnAttackEnded()
{
	if(ActiveAttack != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("%s attack of %s has ended"), *ActiveAttack->GetAttackTypeID().ToString(), *GetNameSafe(GetOwner()))
		ActiveAttack = nullptr;
	}
	UFPFunctionLibrary::NativeRemoveGameplayTagFromActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
}

int32 UCombatComponentBase::GetIndexOfAttack(UFPAttackType* InAttackType) const
{
	if(!InAttackType)
	{
		return INDEX_NONE;
	}

	return AvailableAttackTypes.IndexOfByPredicate([&InAttackType](const UFPAttackType* AttackType)
	{
		return AttackType->GetAttackTypeTag() == InAttackType->GetAttackTypeTag();
	});
}

bool UCombatComponentBase::TryActivateAttack_Implementation(UFPAttackType* InAttackTypeToActivate)
{
	if(InAttackTypeToActivate && AvailableAttackTypes.Contains(InAttackTypeToActivate))
	{
		if(CanAttack() && InAttackTypeToActivate->CanAttack())
		{
			UE_LOG(LogTemp, Display, TEXT("Performing %s attack for %s"), *InAttackTypeToActivate->GetAttackTypeID().ToString(), *GetNameSafe(GetOwner()));
			ActiveAttack = InAttackTypeToActivate;
			UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
			InAttackTypeToActivate->PerformAttack();
			return true;
		}
	}
	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponentBase.h"

#include "CoreTypes/AttackTypes/FPAttackType.h"
#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"

DEFINE_LOG_CATEGORY(LogFpCombat);

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
		UE_LOG(LogFpCombat, Warning, TEXT("Can't add an attack type for %s! The class is invalid!"), *GetNameSafe(GetOwner()));
		return nullptr;
	}

	// Creates attack, subscribe on its delegate and return the instance
	if(UFPAttackType* AttackType = NewObject<UFPAttackType>(this, InAttackTypeClass))
	{
		AvailableAttackTypes.AddUnique(AttackType);
		UE_LOG(LogFpCombat, Display, TEXT("Added %s to %s attacks"), *AttackType->GetAttackTypeID().ToString(), *GetNameSafe(GetOwner()));

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
		// Removes the attack from the list
		AvailableAttackTypes.Remove(FoundAttackType);
		//Ends the attack, if it's active
		if(FoundAttackType->IsActive())
		{
			FoundAttackType->EndAttack(); 
		}
		// Unbinds the attack's delegate
		FoundAttackType->OnAttackEnded.RemoveDynamic(this, &ThisClass::OnAttackEnded);
		// Mark to destroy the attack
		FoundAttackType->ConditionalBeginDestroy();
	}
}

UFPAttackType* UCombatComponentBase::FindAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass)
{
	
	if(!InAttackTypeClass)
	{
		return nullptr;
	}

	// Finds an attack of the specified InAttackTypeClass class 
	UFPAttackType** FoundAttack = AvailableAttackTypes.FindByPredicate([InAttackTypeClass](const UFPAttackType* AttackType)
	{
		return AttackType && AttackType->GetClass() == InAttackTypeClass;
	});

	// Returns the attack point, if was founded, or nullptr otherwise
	return FoundAttack ? *FoundAttack : nullptr;
}

UFPAttackType* UCombatComponentBase::FindAttackTypeByTag(const FGameplayTag& InAttackTypeTag)
{
	if(!InAttackTypeTag.IsValid())
	{
		return nullptr;
	}

	// Finds an attack with the specified InAttackTypeTag tag
	UFPAttackType** FoundedAttackType = AvailableAttackTypes.FindByPredicate([InAttackTypeTag](const UFPAttackType* AttackType)
	{
		return AttackType && AttackType->GetAttackTypeTag().MatchesTagExact(InAttackTypeTag);
	});
	
	// Returns the attack point, if was founded, or nullptr otherwise
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
		// Clear active attack pointer
		UE_LOG(LogFpCombat, Display, TEXT("%s attack of %s has ended"), *ActiveAttack->GetAttackTypeID().ToString(), *GetNameSafe(GetOwner()))
		ActiveAttack = nullptr;
	}
	// Removes attacking status on the owner
	UFPFunctionLibrary::NativeRemoveGameplayTagFromActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
}

int32 UCombatComponentBase::GetIndexOfAttack(UFPAttackType* InAttackType) const
{
	if(!InAttackType)
	{
		return INDEX_NONE;
	}

	// Finds an attack with the same attack type ID and returns its index
	return AvailableAttackTypes.IndexOfByPredicate([&InAttackType](const UFPAttackType* AttackType)
	{
		return AttackType->GetAttackTypeID() == InAttackType->GetAttackTypeID();
	});
}

bool UCombatComponentBase::TryActivateAttack_Implementation(UFPAttackType* InAttackTypeToActivate)
{
	if(InAttackTypeToActivate && AvailableAttackTypes.Contains(InAttackTypeToActivate))
	{
		if(CanAttack() && InAttackTypeToActivate->CanAttack())
		{
			// Set active attack, adds attacking status tag on owner and performs attack
			UE_LOG(LogFpCombat, Display, TEXT("Performing %s attack for %s"), *InAttackTypeToActivate->GetAttackTypeID().ToString(), *GetNameSafe(GetOwner()));
			ActiveAttack = InAttackTypeToActivate;
			UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
			InAttackTypeToActivate->PerformAttack();
			return true;
		}
	}
	return false;
}

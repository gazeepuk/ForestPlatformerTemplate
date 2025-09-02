// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponentBase.h"

#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"



bool UCombatComponentBase::CanAttack_Implementation() const
{
	return !UFPFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
}

bool UCombatComponentBase::TryAttackByTag_Implementation(const FGameplayTag& InAttackTypeTag)
{
	return false;
}

void UCombatComponentBase::OnAttackEnded()
{
	UFPFunctionLibrary::NativeRemoveGameplayTagFromActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
}

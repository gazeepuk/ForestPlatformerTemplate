// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponentBase.h"

void UCombatComponentBase::OnAttackAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bAttacking = false;
}

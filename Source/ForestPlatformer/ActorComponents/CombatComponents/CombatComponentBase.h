// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponentBase.generated.h"


struct FGameplayTag;
class UFPAttackType;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UCombatComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual bool CanAttack() const { return !bAttacking; }

	virtual bool TryAttackByTag(const FGameplayTag& InAttackTypeTag) { return false; }

	bool bAttacking = false;
	
protected:
	virtual void OnAttackAnimationEnded(UAnimMontage* Montage, bool bInterrupted);
};

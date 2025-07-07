// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPAttackType.h"
#include "PooledAttackType.generated.h"

class UObjectPoolContainer;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UPooledAttackType : public UFPAttackType
{
	GENERATED_BODY()
public:
	virtual void InitAttack(AActor* InOwningActor, UCombatComponentBase* InCombatComponent) override;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UObjectPoolContainer> ObjectPoolContainer;
};

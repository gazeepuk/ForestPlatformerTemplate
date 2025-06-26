// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/CombatComponents/CombatComponentBase.h"
#include "Components/ActorComponent.h"
#include "AICombatComponent.generated.h"


struct FGameplayTag;
class UFPAttackType;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UAICombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual bool TryAttackByTag(const FGameplayTag& InAttackTag) override;
	
	AActor* GetTargetActorFromBlackboard() const;

	UFUNCTION(BlueprintPure)
	UFPAttackType* FindAttackTypeByTag(const FGameplayTag& InAttackTypeTag);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UFPAttackType*> AttackTypes;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UFPAttackType>> DefaultAttackTypeClasses;
};

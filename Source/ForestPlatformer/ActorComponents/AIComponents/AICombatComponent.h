// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/CombatComponents/CombatComponentBase.h"
#include "Components/ActorComponent.h"
#include "AICombatComponent.generated.h"


struct FGameplayTag;
class UFPAttackType;

/**
 * AI-specific combat component that extends base combat functionality with
 * AI-specific features like blackboard, tag-based attack selection
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UAICombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()

public:

	/** Attempts to perform an attack associated with the specified tag */
	virtual bool TryAttackByTag_Implementation(const FGameplayTag& InAttackTypeTag) override;

	/** Retrieves TargetActor from the AI owner's blackboard */
	AActor* GetTargetActorFromBlackboard() const;

	/** Searches for and returns the attack type associated with the specified gameplay tag */
	UFUNCTION(BlueprintPure)
	UFPAttackType* FindAttackTypeByTag(const FGameplayTag& InAttackTypeTag);
	
protected:
	virtual void BeginPlay() override;

	/** Array of all available attack types of this AI owner */
	UPROPERTY(BlueprintReadOnly)
	TArray<UFPAttackType*> AttackTypes;

private:
	/** Default attack type calsses that are available to this AI owner */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UFPAttackType>> DefaultAttackTypeClasses;
};

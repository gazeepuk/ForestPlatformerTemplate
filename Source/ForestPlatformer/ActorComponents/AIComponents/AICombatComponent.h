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

	/** Retrieves TargetActor from the AI owner's blackboard */
	AActor* GetTargetActorFromBlackboard() const;
	
protected:
	virtual void BeginPlay() override;
};

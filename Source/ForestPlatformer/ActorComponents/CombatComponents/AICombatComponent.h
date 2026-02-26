// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ActorComponents/CombatComponents/CombatComponentBase.h"
#include "Components/ActorComponent.h"
#include "AICombatComponent.generated.h"


class UBlackboardComponent;
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
	UFUNCTION(BlueprintPure)
	AActor* GetTargetActorFromBlackboard() const;
	
	/** Retrieves Blackboard component of the AI owner's controller */
	UFUNCTION(BlueprintPure)
	const UBlackboardComponent* GetBlackboardFromOwningActor() const;
	
protected:
	virtual void BeginPlay() override;
};

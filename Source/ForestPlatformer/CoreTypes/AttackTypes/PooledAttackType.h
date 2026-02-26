// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FPAttackType.h"
#include "PooledAttackType.generated.h"

class UObjectPoolContainer;
/**
 * Specialized attack type that utilizing object pooling for efficient spawning and reuse of related actors 
 */
UCLASS()
class FORESTPLATFORMER_API UPooledAttackType : public UFPAttackType
{
	GENERATED_BODY()
public:
	/** Initialize the pooled attack type with ownership and combat component references */
	virtual void InitAttack(AActor* InOwningActor, UCombatComponentBase* InCombatComponent) override;
	/** Cleans up object pool when this attack type is being destroyed */
	virtual void BeginDestroy() override;
protected:

	/** Object pool container that manages the allocation and reuse of actors */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UObjectPoolContainer> ObjectPoolContainer;
};

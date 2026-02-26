// Developed by Ivan Piankouski. All Rights Reserved


#include "PooledAttackType.h"

#include "ObjectPool/ObjectPoolComponent.h"

void UPooledAttackType::InitAttack(AActor* InOwningActor, UCombatComponentBase* InCombatComponent)
{
	Super::InitAttack(InOwningActor, InCombatComponent);

	if(ObjectPoolContainer)
	{
		ObjectPoolContainer->InitializePool(InOwningActor, InOwningActor);
	}
}

void UPooledAttackType::BeginDestroy()
{
	if(ObjectPoolContainer)
	{
		ObjectPoolContainer->ClearPool();
	}
	
	Super::BeginDestroy();
}

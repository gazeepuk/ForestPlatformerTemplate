// Fill out your copyright notice in the Description page of Project Settings.


#include "AICombatComponent.h"

#include "AIController.h"
#include "AttackTypes/FPAttackType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "GameFramework/Character.h"

bool UAICombatComponent::TryAttackByTag_Implementation(const FGameplayTag& InAttackTypeTag)
{
	if(UFPAttackType* AttackType = FindAttackTypeByTag(InAttackTypeTag))
	{
		if(CanAttack() && AttackType->CanAttack())
		{
			UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
			AttackType->PerformAttack(GetTargetActorFromBlackboard());
			return true;
		}
	}
	
	return false;
}

AActor* UAICombatComponent::GetTargetActorFromBlackboard() const
{
	if(const ACharacter* OwningCharacter = GetOwner<ACharacter>())
	{
		if(const AAIController* AIController = OwningCharacter->GetController<AAIController>())
		{
			if(const UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
			{
				return Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("TargetActor")));
			}
		}
	}
	return nullptr;
}

UFPAttackType* UAICombatComponent::FindAttackTypeByTag(const FGameplayTag& InAttackTypeTag)
{
	if(!InAttackTypeTag.IsValid())
	{
		return nullptr;
	}
	
	UFPAttackType** FoundedAttackType = AttackTypes.FindByPredicate([InAttackTypeTag](const UFPAttackType* AttackType)
	{
		return AttackType && AttackType->GetAttackTypeTag().MatchesTagExact(InAttackTypeTag);
	});

	return FoundedAttackType ? *FoundedAttackType : nullptr;
}

void UAICombatComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UFPAttackType>& AttackTypeClass : DefaultAttackTypeClasses)
	{
		if(!AttackTypeClass)
		{
			continue;
		}
		
		if(UFPAttackType* AttackType = NewObject<UFPAttackType>(GetOwner(), AttackTypeClass))
		{
			AttackType->InitAttack(GetOwner(), this);
			AttackType->OnAttackEnded.AddUniqueDynamic(this, &ThisClass::OnAttackEnded);
			AttackTypes.AddUnique(AttackType);
		}
	}
}
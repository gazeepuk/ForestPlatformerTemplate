// Fill out your copyright notice in the Description page of Project Settings.


#include "AICombatComponent.h"

#include "AIController.h"
#include "CoreTypes/AttackTypes/FPAttackType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "GameFramework/Character.h"


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

void UAICombatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAttacks();
}
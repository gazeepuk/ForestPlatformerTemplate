// Developed by Ivan Piankouski. All Rights Reserved


#include "AICombatComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


AActor* UAICombatComponent::GetTargetActorFromBlackboard() const
{
	// Tries to get TargetActor value from owner blackboard
	if(const UBlackboardComponent* BlackboardComponent = GetBlackboardFromOwningActor())
	{
		return Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("TargetActor")));
	}
	return nullptr;
}

const UBlackboardComponent* UAICombatComponent::GetBlackboardFromOwningActor() const
{
	if(const ACharacter* OwningCharacter = GetOwner<ACharacter>())
	{
		if(const AAIController* AIController = OwningCharacter->GetController<AAIController>())
		{
			return AIController->GetBlackboardComponent();
		}
	}
	return nullptr;
}

void UAICombatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAttacks();
}

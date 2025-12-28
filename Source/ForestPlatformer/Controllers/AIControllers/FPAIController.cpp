 // Fill out your copyright notice in the Description page of Project Settings.


#include "FPAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CoreTypes/GameplayTags/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"	

AFPAIController::AFPAIController(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// Setups sight sense config
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	SenseConfig_Sight->SightRadius = 2000.f;
	SenseConfig_Sight->LoseSightRadius = 3000.f;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 150.f;

	// Adds perception component
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnPerceptionUpdated);

	// Sets enemy team 
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AFPAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if(!OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}
	
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());
	if(OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AFPAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Sight>())
	{
		HandleSightSense(Actor, Stimulus);
	}
}

void AFPAIController::HandleSightSense_Implementation(AActor* InActor, FAIStimulus InStimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	ETeamAttitude::Type TeamAttitudeTowardsTargetActor = GetTeamAttitudeTowards(*InActor);
	
	if(BlackboardComponent && TeamAttitudeTowardsTargetActor == ETeamAttitude::Type::Hostile)
	{
		AActor* TargetActor = UFPFunctionLibrary::NativeDoesActorHaveTag(InActor, FPGameplayTags::Shared_Status_Dead) ? nullptr : InActor;
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), InStimulus.WasSuccessfullySensed() ? TargetActor : nullptr);
		BlackboardComponent->SetValueAsVector(FName("LastKnownLocation"), InStimulus.StimulusLocation);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetNextPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "CoreTypes/PatrolTypes/PatrolDataTypes.h"

UBTTask_GetNextPatrolPoint::UBTTask_GetNextPatrolPoint()
{
	NodeName = TEXT("Native Get Next Patrol Point");

	bNotifyTick = false;
	bNotifyTaskFinished = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InNextPatrolPointKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InNextPatrolPointKey));
}

void UBTTask_GetNextPatrolPoint::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if(UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InNextPatrolPointKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTTask_GetNextPatrolPoint::GetStaticDescription() const
{
	const FString KeyDescription = InNextPatrolPointKey.SelectedKeyName.ToString();
	return TEXT("Sets next patrol location to ") + KeyDescription;
}

EBTNodeResult::Type UBTTask_GetNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(!PatrolData)
	{
		return EBTNodeResult::Failed;
	}
	
	FVector NextPatrolLocation = FVector::ZeroVector;
	if(PatrolData->GetNextPatrolLocation(OwnerComp.GetAIOwner()->GetPawn(), NextPatrolLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(InNextPatrolPointKey.SelectedKeyName, NextPatrolLocation);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextPatrolPoint.generated.h"

class UPatrolDataBase;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UBTTask_GetNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetNextPatrolPoint();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Patrolling")
	FBlackboardKeySelector InNextPatrolPointKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Patrolling")
	TObjectPtr<UPatrolDataBase> PatrolData;
};

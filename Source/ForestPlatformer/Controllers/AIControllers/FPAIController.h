// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "FPAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class FORESTPLATFORMER_API AFPAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFPAIController(const FObjectInitializer& ObjectInitializer);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> SenseConfig_Sight;
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleSightSense(AActor* InActor, FAIStimulus InStimulus);

private:
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};

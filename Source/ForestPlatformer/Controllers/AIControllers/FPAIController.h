// Developed by Ivan Piankouski. All Rights Reserved

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

	/** Returns the team ID associated with the AI controller */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	/** Component responsible for detecting and processing perceptual stimuli */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

	/** Configuration for the AI's visual perception */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> SenseConfig_Sight;

	/** Handles visual perception updates */
	UFUNCTION(BlueprintNativeEvent)
	void HandleSightSense(AActor* InActor, FAIStimulus InStimulus);

private:
	/** Callback function triggered when any perception update occurs */
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
};

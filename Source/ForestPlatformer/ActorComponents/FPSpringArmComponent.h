// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "FPSpringArmComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UFPSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	void SetTargetArmLengthLerp(float InNewTargetArmLength, float InLerpDuration = 1.5f);
	void AddTargetArmLength(float InAdditiveValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float ZoomingStep = 50.f;
	
protected:
	virtual void InitializeComponent() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength")
	bool bClampTargetArmLength = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float MinTargetArmLength = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float MaxTargetArmLength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float TargetArmLengthLerpDuration = 1.5f;
	
	
private:
	FTimerHandle SetTargetArmLengthLerpHandle;

	float FinalTargetArmLength;
	float LerpStartTime;
	
	void UpdateTargetArmLength();
};

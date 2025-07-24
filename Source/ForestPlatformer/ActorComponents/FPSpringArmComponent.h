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
	UFPSpringArmComponent();
	void SetTargetArmLengthLerp(float InNewTargetArmLength, float InLerpDuration = 1.5f);
	void AddTargetArmLength(float InAdditiveValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float ZoomingStep = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength")
	bool bClampTargetArmLength = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float MinTargetArmLength = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float MaxTargetArmLength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|TargetArmLength", meta = (ClampMin = "0"))
	float TargetArmLengthLerpDuration = 1.5f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentSpeed = 1.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentFrontAngleThreshold = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentBackAngleThreshold = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float VelocityAlignmentThreshold = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment")
	bool bEnableMovementAlignment = false;
	
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateTargetArmLength();
	void UpdateMovementAlignment(float InDeltaTime);
	
	FTimerHandle SetTargetArmLengthLerpHandle;

	float FinalTargetArmLength;
	float LerpStartTime;

	TWeakObjectPtr<APawn> OwnerPawn;
	TWeakObjectPtr<AController> OwnerController;
};

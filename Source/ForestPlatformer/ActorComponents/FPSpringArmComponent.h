// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "FPSpringArmComponent.generated.h"

/**
 * Enhanced spring arm component with additional features for camera control,
 * including smooth zooming, length clamping and movement-based alignment
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UFPSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UFPSpringArmComponent();

	/**
	 * Smoothly interpolates the target arm length to a new value over specified duration
	 * @param InNewTargetArmLength The desired final arm length
	 * @param InLerpDuration Duration of the interpolation in seconds
	 */
	void SetTargetArmLengthLerp(float InNewTargetArmLength, float InLerpDuration = 1.5f);

	/** Adds a value to the current target arm length */
	void AddTargetArmLength(float InAdditiveValue);

	/** Step size for incremental zoom adjustments */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|Zooming", meta = (ClampMin = "0"))
	float ZoomingStep = 100.f;

	/** Whether to clamp the target arm length between minimum and maximum values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|Zooming")
	bool bClampTargetArmLength = false;

	/** Minimum allowed arm length when clamping is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|Zooming", meta = (ClampMin = "0"))
	float MinTargetArmLength = 50.f;
	/** Maximum allowed arm length when clamping is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|Zooming", meta = (ClampMin = "0"))
	float MaxTargetArmLength = 600.f;

	/**	Duration for arm length interpolation when using smooth zooming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|Zooming", meta = (ClampMin = "0"))
	float TargetArmLengthLerpDuration = 1.5f;

	/**	Speed factor for movement-based camera alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentSpeed = 1.5;
	/** Angle threshold (in degrees) for forward movement alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentFrontAngleThreshold = 30.f;
	/** Angle threshold (in degrees) for backward movement alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentBackAngleThreshold = 80.f;
	/** Minimum velocity required for movement alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment", meta = (ClampMin = "0"))
	float VelocityAlignmentThreshold = 10.f;
	/** Whether the movement-based camera alignment is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintArm|MovementAlignment")
	bool bEnableMovementAlignment = false;
	
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Updates the arm length through interpolation */
	void UpdateTargetArmLength();
	/** Adjust camera alignment based on character movement direction and velocity */
	void UpdateMovementAlignment(float InDeltaTime);

	/** Timer handle for managing arm length interpolation*/
	FTimerHandle SetTargetArmLengthLerpHandle;

	/** The target arm length to interpolate towards */
	float FinalTargetArmLength;
	/** The start time of the current interpolation */
	float LerpStartTime;

	/** Weak reference to the owning pawn  */
	TWeakObjectPtr<APawn> OwnerPawn;
	/** Weak reference to the owning controller */
	TWeakObjectPtr<AController> OwnerController;
};

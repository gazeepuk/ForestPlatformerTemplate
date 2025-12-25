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

	/** Adds a value to the current target arm length */
	void AddTargetArmLength(float InAdditiveLength);

	/** Stops zooming and immidietly make TargetArmLength equal	to FinalTargetArmLength  */
	void StopZooming();

	/**
	 * Zooms in the specified direction for mouse
	 * @param InZoomingMultiplier Zooming multiplier ( < 0.f - zoom out, == 0.f - do nothing, > 0.f - zoom in);
	 */
	void ZoomMouse(float InZoomingMultiplier);
	/**
	 * Zooms in the specified direction for controller
	 * @param InZoomingMultiplier Zooming multiplier ( < 0.f - zoom out, == 0.f - do nothing, > 0.f - zoom in);
	 */
	void ZoomController(float InZoomingMultiplier);
	
	/** Step size for incremental zoom adjustments for mouse*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|Zooming", meta = (ClampMin = "0"))
	float MouseZoomingStep = 100.f;

	/** Step size for incremental zoom adjustments for controller*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|Zooming", meta = (ClampMin = "0"))
	float ControllerZoomingStep = 10.f;

	/** Zooming speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|Zooming", meta = (ClampMin = "0"))
	float ZoomingSpeed = 1.5f;
	
	/** Whether to clamp the target arm length between minimum and maximum values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|Zooming")
	bool bClampTargetArmLength = false;

	/** Minimum allowed arm length when clamping is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|Zooming", meta = (ClampMin = "0"))
	float MinTargetArmLength = 50.f;
	/** Maximum allowed arm length when clamping is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|Zooming", meta = (ClampMin = "0"))
	float MaxTargetArmLength = 600.f;
	

	/**	Speed factor for movement-based camera alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|MovementAlignment", meta = (ClampMin = "0"))
	float MovementAlignmentSpeed = 1.5;
	/** Angle threshold (in degrees) for forward movement alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|MovementAlignment", meta = (ClampMin = "0", ClampMax = "180"))
	float MovementAlignmentFrontAngleThreshold = 30.f;
	/** Angle threshold (in degrees) for backward movement alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|MovementAlignment", meta = (ClampMin = "0", ClampMax = "180"))
	float MovementAlignmentBackAngleThreshold = 100.f;
	/** Minimum velocity required for movement alignment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|MovementAlignment", meta = (ClampMin = "0"))
	float VelocityAlignmentThreshold = 10.f;
	/** Whether the movement-based camera alignment is enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|MovementAlignment")
	bool bEnableMovementAlignment = false;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm|MovementAlignment", meta = (ClampMin = "0"))
	float SnappingDistance = 1.f;
private:
	/** Adjust camera alignment based on character movement direction and velocity */
	void UpdateMovementAlignment(float InDeltaTime);

	/** Timer handle for managing arm length interpolation*/
	FTimerHandle SetTargetArmLengthLerpHandle;

	/** The target arm length to interpolate towards */
	float FinalTargetArmLength;
	
	/** Weak reference to the owning pawn  */
	TWeakObjectPtr<APawn> OwnerPawn;
	/** Weak reference to the owning controller */
	TWeakObjectPtr<AController> OwnerController;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SceneRotatingMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API USceneRotatingMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USceneRotatingMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FVector RotationAxis = FVector::UpVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationSpeed = 90.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bRotationEnabled = true;
	
	UPROPERTY(BlueprintReadWrite, Category = "Rotation")
	TObjectPtr<USceneComponent> TargetComponent;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPCharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UFPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	void StartFloating();
	void StopFloating();
	void HandleFloating();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Falling")
	float FloatingSpeedMultiplier = 1.f;

private:
	bool bFloating;
};

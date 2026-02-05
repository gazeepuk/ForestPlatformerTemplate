// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API ULaunchingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LaunchInDirection(FVector InDirection, float InLaunchingForce = -1.f);

	UFUNCTION(BlueprintCallable)
	void LaunchFromImpactPoint(FVector InImpactPoint, float InLaunchingForce = -1.f);

	UFUNCTION(BlueprintCallable)
	void LaunchWithParams(FVector InDirectionVector, float InLaunchStrength, bool bResolveDirection = true);

	UFUNCTION(BlueprintPure)
	FVector ResolveLaunchingDirection(const FVector& InRawDirection) const;

protected:
	UFUNCTION(BlueprintPure)
	bool IsOwnerGrounded() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Launching")
	float DefaultLaunchForce = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Launching")
	bool bUsePawnEyes = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Launching")
	bool bResolveLaunchingDirection = true;
};

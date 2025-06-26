// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	bool IsCurrentPatrolIndexValid() const;
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsPatrolBordersValid() const { return PatrolBorders != nullptr; } 
	
	UFUNCTION(BlueprintPure)
	FVector GetPatrolPointLocationByIndex(int32 InPatrolPointIndex) const;
	UFUNCTION(BlueprintPure)
	FVector GetCurrentPatrolPointLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetRandomLocationWithinPatrolBorders() const;
	
	UFUNCTION(BlueprintCallable)
	void SetNextPatrolPointIndex();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> PatrolPoints;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPatrolPointIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPatrolForward = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AVolume> PatrolBorders;
};

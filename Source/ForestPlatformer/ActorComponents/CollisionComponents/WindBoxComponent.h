// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "WindBoxComponent.generated.h"


class UArrowComponent;

/**
 * Specialized collision that creates upward wind currents
 * to lift characters when they enter the volume
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UWindBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UWindBoxComponent();
	
protected:
	/** Speed at which actors are lifted upward within the wind volume */
	UPROPERTY(EditAnywhere, Category = "Lifting")
	float LiftSpeed = 15000.f;
	
	/**	Controls whether the wind lifting effect is currently active */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lifting")
	bool bLifting = true;

	/**	Callback function triggered when an actor enters the wind volume */
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/**	Callback function triggered when an actor leaves the wind volume */
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/** Applies upward force to all overlapping actors each frame when lifting is active */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Array of actors currently within the wind volume */
	UPROPERTY()
	TArray<AActor*> OverlappedActors;
};

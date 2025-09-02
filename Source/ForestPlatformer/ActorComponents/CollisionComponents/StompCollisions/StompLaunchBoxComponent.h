// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StompBoxComponent.h"
#include "StompLaunchBoxComponent.generated.h"

/**
 * Specific stomp collision component that launches actors upwards
 * when they are stomped on from above
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UStompLaunchBoxComponent : public UStompBoxComponent
{
	GENERATED_BODY()
	
protected:
	/** Force magnitude applied to actors when launched upward after being stomped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchActorForce = 1000.f;
	
private:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

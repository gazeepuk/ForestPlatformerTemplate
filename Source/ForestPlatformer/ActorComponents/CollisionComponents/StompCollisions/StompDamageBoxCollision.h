// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StompLaunchBoxComponent.h"
#include "StompDamageBoxCollision.generated.h"


/**
 * Specific stomp collision component that applies damage to its owning actor
 * when overlapped by character from above
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UStompDamageBoxCollision : public UStompLaunchBoxComponent
{
	GENERATED_BODY()
public:
	UStompDamageBoxCollision();
	
protected:

	/** Amount of damage to apply to the owner when stomped */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageAmount = 1.f;
	
private:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

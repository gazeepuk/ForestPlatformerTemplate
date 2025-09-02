// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructiblePlatform.generated.h"

class UStompBoxComponent;
class UStompLaunchBoxComponent;

UCLASS()
class FORESTPLATFORMER_API ADestructiblePlatform : public AActor
{
	GENERATED_BODY()

public:
	ADestructiblePlatform();

protected:
	/* Stomp collision to detect a player */
	TObjectPtr<UStompBoxComponent> StompBoxComponent;

	/* Determines if the platform get destroyed after interaction */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDestroyAfter = false;

	/* Destruction delay after the interaction with the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestructionTime = 5.f;
	/* Restoration delay after destructing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestorationTime = 5.f;

private:
	UFUNCTION()
	void OnStompBoxOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Destroys or disables the platform */
	void DestroyPlatform();
	/* Restores the platform */
	void RestorePlatform();
	
	FTimerHandle DestructionTimerHandle;
	FTimerHandle RestorationTimerHandle;

	/** Indicates whether the platform is already destructing */
	bool bDestructing;
};

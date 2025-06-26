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
	TObjectPtr<UStompBoxComponent> StompBoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDestroyAfter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestructionTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestorationTime = 5.f;

private:
	UFUNCTION()
	void OnStompBoxOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroyPlatform();
	void RestorePlatform();
	
	FTimerHandle DestructionTimerHandle;
	FTimerHandle RestorationTimerHandle;

	bool bDestructing;
};

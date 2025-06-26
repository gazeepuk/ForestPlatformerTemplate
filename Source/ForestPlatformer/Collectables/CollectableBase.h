// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableBase.generated.h"

class UBoxComponent;

UCLASS()
class FORESTPLATFORMER_API ACollectableBase : public AActor
{
	GENERATED_BODY()

public:
	ACollectableBase();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxCollision;

	UFUNCTION(BlueprintNativeEvent)
	void OnCollectableOverlapped(AActor* InOverlappedActor);

	UPROPERTY(EditDefaultsOnly, Category = "Collectable")
	bool bDestroyOnCollect = true;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bDestroyOnCollect"), Category = "Collectable")
	bool bEnableCollectableInTime = true;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bDestroyOnCollect && bEnableCollectableInTime"), Category = "Collectable")
	float CollectableRespawnTime = 30.f;

	UFUNCTION(BlueprintNativeEvent, Category = "Collectable")
	bool CanCollect(AActor* InInstigator);
private:
	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FTimerHandle CoinRespawnTimerHandle;

	UFUNCTION()
	void RespawnCollectable();
};

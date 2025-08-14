// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalVolume.generated.h"

class UBoxComponent;

UCLASS()
class FORESTPLATFORMER_API ADecalVolume : public AActor
{
	GENERATED_BODY()

public:
	ADecalVolume();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void MatchDecalSize() const;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> DecalComponent;
};

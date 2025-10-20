// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalVolume.generated.h"

class UBoxComponent;

/**
 * Actor that combines a box volume with a decal component, automatically matching
 * the decal size to the box dimensions	for easy placement and scaling 
 */
UCLASS()
class FORESTPLATFORMER_API ADecalVolume : public AActor
{
	GENERATED_BODY()

public:
	ADecalVolume();
	/** Called during construction in the editor to update component properties */
	virtual void OnConstruction(const FTransform& Transform) override;
	/** Matches the decal size to the box component extent */
	UFUNCTION(BlueprintCallable)
	void MatchDecalSize() const;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** Box component that defines the volume boundaries and collision area */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	/** Decal component that projects materials within the volume boundaries */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> DecalComponent;
};

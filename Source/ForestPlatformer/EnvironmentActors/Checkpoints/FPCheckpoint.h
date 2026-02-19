// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableActorInterface.h"
#include "FPCheckpoint.generated.h"

class UArrowComponent;
class UBoxComponent;
UCLASS()
class FORESTPLATFORMER_API AFPCheckpoint : public AActor, public ISavableActorInterface
{
	GENERATED_BODY()

public:
	AFPCheckpoint();

	/* Activates or deactivates this checkpoint */
	UFUNCTION(BlueprintCallable)
	void SetCheckpointActivated(bool bNewActivated);
	/* returns the transform where the player should spawn */
	UFUNCTION(BlueprintPure)
	FTransform GetSpawnPointTransform() const;
	
	//~Begin ISavableObjectInterface
	/** Initializes the Save ID for this actor */
	UFUNCTION(CallInEditor, Category = "SavableObject | HelperFunctions")
	virtual void InitializeSaveID() override { ISavableActorInterface::InitializeSaveID(); }
	/** Returns the unique save identifier of this actor */
	virtual FName GetSaveID_Implementation() const override;
	/** Sets the unique save identifier for this actor */
	virtual void SetSaveID_Implementation(const FName& InSaveID) override { SaveID = InSaveID; }
	/** Determines whether the actor should be saved */
	virtual bool ShouldSave_Implementation() const override;
	/** Generates save data containing the current state of this actor */
	virtual FFPSavableData GetSaveData_Implementation() const override;
	/** Loads the actor's state from save data */
	virtual void LoadFromSaveData_Implementation(const FFPSavableData& SaveData) override;
	/** Called after the actor has been loaded from save data */
	virtual void OnLoadedFromSaveData_Implementation() override {}
	//~End ISavableObjectInterface

protected:
	/** Checkpoint collision that detects when the player enter the checkpoint area */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> CheckpointBoundaries;

	/** Component that detects the exact place where the player should be spawned */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SpawnPoint;

	/** Visual arrow component indicating the spawn direction for the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> SpawnDirectionArrow;

	/** Unique identifier used for saving and loading */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SaveID;

	/** Indicates whether this checkpoint is activated */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bActivated;

	/** Callback function triggered when the checkpoint boundaries overlap events */
	UFUNCTION()
	void OnPlayerOverlappedBoundaries(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Blueprint event called when this checkpoint becomes activated */
	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointActivated();
};

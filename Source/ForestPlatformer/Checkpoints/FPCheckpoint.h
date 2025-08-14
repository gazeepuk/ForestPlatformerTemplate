// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableObjectInterface.h"
#include "FPCheckpoint.generated.h"

class UArrowComponent;
class UBoxComponent;
UCLASS()
class FORESTPLATFORMER_API AFPCheckpoint : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()

public:
	AFPCheckpoint();
	UFUNCTION(BlueprintCallable)
	void SetCheckpointActivated(bool bNewActivated);
	UFUNCTION(BlueprintPure)
	FTransform GetSpawnPointTransform() const;
	
	//~Begin ISavableObjectInterface
	UFUNCTION(CallInEditor, Category = "SavableObject | HelperFunctions")
	virtual void InitializeSaveID() override { ISavableObjectInterface::InitializeSaveID(); }
	virtual FName GetSaveID_Implementation() const override;
	virtual void SetSaveID_Implementation(const FName& InSaveID) override { SaveID = InSaveID; }
	virtual bool ShouldSave_Implementation() const override;
	virtual FFPSavableData GetSaveData_Implementation() const override;
	virtual void LoadFromSaveData_Implementation(const FFPSavableData& SaveData) override;
	virtual void OnLoadedFromSaveData_Implementation() override {}
	//~End ISavableObjectInterface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> CheckpointBoundaries;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> SpawnDirectionArrow;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SaveID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bActivated;

	UFUNCTION()
	void OnPlayerOverlappedBoundaries(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointActivated();
};

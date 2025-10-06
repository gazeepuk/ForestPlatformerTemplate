// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableActorInterface.h"
#include "CollectableBase.generated.h"

class UBoxComponent;

UCLASS()
class FORESTPLATFORMER_API ACollectableBase : public AActor, public ISavableActorInterface
{
	GENERATED_BODY()

public:
	ACollectableBase();

	//~Begin ISavableActorInterface
	/** Initializes the Save ID for this actor */
	UFUNCTION(CallInEditor, Category = "SavableObject | HelperFunctions")
	virtual void InitializeSaveID() override { ISavableActorInterface::InitializeSaveID(); }
	/** Returns the unique save identifier of this actor */
	virtual FName GetSaveID_Implementation() const override { return SaveID; }
	/** Sets the unique save identifier for this actor */
	virtual void SetSaveID_Implementation(const FName& InSaveID) override { SaveID = InSaveID; }
	/** Determines whether the actor should be saved */
	virtual bool ShouldSave_Implementation() const override { return bSaveOnCollected; }
	/** Generates save data containing the current state of this actor */
	virtual FFPSavableData GetSaveData_Implementation() const override;
	/** Loads the actor's state from save data */
	virtual void LoadFromSaveData_Implementation(const FFPSavableData& SaveData) override;
	/** Called after the enemy has been loaded from save data */
	virtual void OnLoadedFromSaveData_Implementation() override;
	//~End ISavableActorInterface
	
protected:
	
	/** Collectable collision */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxCollision;

	virtual void NativeOnCollected(AActor* InInstigator) {}
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Collected")
	void BP_OnCollected(AActor* InInstigator);
	
	/** If true, the collectable will be destroyed when collected */
	UPROPERTY(EditAnywhere, Category = "Collectable")
	bool bDestroyOnCollect = true;

	/** If true and bDestroyOnCollect is false, the collectable will re-enable after a delay */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bDestroyOnCollect"), Category = "Collectable")
	bool bEnableCollectableInTime = true;
	/** Time in seconds before the collectable re-enables */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bDestroyOnCollect && bEnableCollectableInTime"), Category = "Collectable")
	float CollectableRespawnTime = 30.f;

	/** Determines if the specified actor can collect this collectable */
	UFUNCTION(BlueprintNativeEvent, Category = "Collectable")
	bool CanCollect(AActor* InInstigator);

	/** Indicates where this collectable is currently active */
	UPROPERTY(BlueprintReadWrite)
	bool bActive;

	/** Unique identifier for saving and loading this collectable's state */
	UPROPERTY(EditAnywhere, Category = "SavableObject")
	FName SaveID;

	/** If true, this collectable will be after being collected */
	UPROPERTY(EditAnywhere, Category = "SavableObject")
	bool bSaveOnCollected = false;

private:
	/** Called after collecting the collectable. */
	void HandleCollection();

	/** Callback function for the box component overlap event */
	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FTimerHandle CollectableRespawnTimerHandle;

	/** Reactivates this collectable */
	UFUNCTION()
	void RespawnCollectable();
};

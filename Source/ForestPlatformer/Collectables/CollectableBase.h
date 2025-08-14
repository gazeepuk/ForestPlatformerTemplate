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

	UFUNCTION(CallInEditor, Category = "SavableObject|Helper Functions")
	virtual void InitializeSaveID() override { ISavableActorInterface::InitializeSaveID(); }
	virtual FName GetSaveID_Implementation() const override { return SaveID; }
	virtual void SetSaveID_Implementation(const FName& InSaveID) override { SaveID = InSaveID; }
	virtual bool ShouldSave_Implementation() const override { return bSaveOnCollected; }
	virtual FFPSavableData GetSaveData_Implementation() const override;
	virtual void LoadFromSaveData_Implementation(const FFPSavableData& SaveData) override;
	virtual void OnLoadedFromSaveData_Implementation() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxCollision;

	UFUNCTION(BlueprintNativeEvent)
	void OnCollectableOverlapped(AActor* InOverlappedActor);

	UPROPERTY(EditAnywhere, Category = "Collectable")
	bool bDestroyOnCollect = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bDestroyOnCollect"), Category = "Collectable")
	bool bEnableCollectableInTime = true;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bDestroyOnCollect && bEnableCollectableInTime"), Category = "Collectable")
	float CollectableRespawnTime = 30.f;

	UFUNCTION(BlueprintNativeEvent, Category = "Collectable")
	bool CanCollect(AActor* InInstigator);

	UPROPERTY(BlueprintReadWrite)
	bool bActive;
	
	UPROPERTY(EditAnywhere, Category = "SavableObject")
	FName SaveID;

	UPROPERTY(EditAnywhere, Category = "SavableObject")
	bool bSaveOnCollected = false;
private:
	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FTimerHandle CoinRespawnTimerHandle;

	UFUNCTION()
	void RespawnCollectable();
};

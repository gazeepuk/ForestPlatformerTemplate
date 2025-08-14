// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPCharacterBase.h"
#include "Interfaces/SavableObjectInterface.h"
#include "FPEnemyCharacter.generated.h"

class UFPEffectComponent;

UCLASS()
class FORESTPLATFORMER_API AFPEnemyCharacter : public AFPCharacterBase, public IDamageableInterface, public ISavableObjectInterface
{
	GENERATED_BODY()

public:
	AFPEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatedBy) override;

	UFUNCTION(CallInEditor, Category = "SavableObject|Helper Functions")
	virtual void InitializeSaveID() override { ISavableObjectInterface::InitializeSaveID(); }
	virtual FName GetSaveID_Implementation() const override { return SaveID; }
	virtual void SetSaveID_Implementation(const FName& InSaveID) override { SaveID = InSaveID; }
	virtual bool ShouldSave_Implementation() const override { return bDefeated && bSaveAfterDefeating; }
	virtual FFPSavableData GetSaveData_Implementation() const override;
	virtual void LoadFromSaveData_Implementation(const FFPSavableData& SaveData) override;
	virtual void OnLoadedFromSaveData_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void DisableEnemy();
	
protected:
	virtual void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPEffectComponent> EffectComponent;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDefeated;
	
	UPROPERTY(EditAnywhere, Category = "SavableObject")
	FName SaveID;

	UPROPERTY(EditAnywhere, Category = "SavableObject")
	bool bSaveAfterDefeating = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyAfterDefeating = true;
};

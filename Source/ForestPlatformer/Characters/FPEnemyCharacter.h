// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPCharacterBase.h"
#include "Interfaces/SavableActorInterface.h"
#include "FPEnemyCharacter.generated.h"

class UAICombatComponent;
class UFPEffectComponent;

UCLASS()
class FORESTPLATFORMER_API AFPEnemyCharacter : public AFPCharacterBase, public IDamageableInterface, public ISavableActorInterface
{
	GENERATED_BODY()

public:
	AFPEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	//~Begin IDamageableInterface
	/**
	 * Applies damage to the enemy character
	 * @param DamageCauser The actor that caused the damage
	 * @param InDamage The amount of damage to apply
	 * @param InstigatedBy The controller responsible for the damage
	 */
	virtual void TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatedBy) override;
	//~End IDamageableInterface

	//~Begin ISavableActorInterface
	/** Initializes the Save ID for this actor */
	UFUNCTION(CallInEditor, Category = "SavableObject | HelperFunctions")
	virtual void InitializeSaveID() override { ISavableActorInterface::InitializeSaveID(); }
	/** Returns the unique save identifier of this actor */
	virtual FName GetSaveID_Implementation() const override { return SaveID; }
	/** Sets the unique save identifier for this actor */
	virtual void SetSaveID_Implementation(const FName& InSaveID) override { SaveID = InSaveID; }
	/** Determines whether the actor should be saved */
	virtual bool ShouldSave_Implementation() const override { return bDefeated && bSaveAfterDefeating; }
	/** Generates save data containing the current state of this actor */
	virtual FFPSavableData GetSaveData_Implementation() const override;
	/** Loads the actor's state from save data */
	virtual void LoadFromSaveData_Implementation(const FFPSavableData& SaveData) override;
	/** Called after the enemy has been loaded from save data */
	virtual void OnLoadedFromSaveData_Implementation() override;
	//~End ISavableActorInterface
	
	/** Disable the enemy, typically called after defeat */
	UFUNCTION(BlueprintCallable)
	void DisableEnemy();
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	/**
	 * Handles enemy's death
	 * @note Always call the parent function!
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDeath();

	/** Callback function called whenever the enemy takes damage */
	UFUNCTION(BlueprintNativeEvent)
	void OnTakeDamage(float DamageValue);
	
	/** Component responsible for managing enemy's health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;

	/** Component responsible for managing effects on the enemy */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPEffectComponent> EffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAICombatComponent> AICombatComponent;
	
	/** Indicates whether the enemy is defeated */
	UPROPERTY(BlueprintReadWrite)
	bool bDefeated;

	/** Unique identifier of the enemy for saving and loading */
	UPROPERTY(EditAnywhere, Category = "SavableObject")
	FName SaveID;

	/** If true, the enemy will be saved after being destroyed */
	UPROPERTY(EditAnywhere, Category = "SavableObject")
	bool bSaveAfterDefeating = false;

	/** If true, the enemy will be destroyed after being defeated */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyAfterDefeating = true;
};

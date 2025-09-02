// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/FPSavableDataTypes.h"
#include "UObject/Interface.h"
#include "SavableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USavableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FORESTPLATFORMER_API ISavableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Initializes the Save ID for this actor */
	virtual void InitializeSaveID();
	/** Returns the unique save identifier of this actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	FName GetSaveID() const;
	/** Sets the unique save identifier for this actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	void SetSaveID(const FName& InSaveID);
	/** Determines whether the actor should be saved */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	bool ShouldSave() const;
	/** Generates save data containing the current state of this actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	FFPSavableData GetSaveData() const;
	/** Loads the actor's state from save data */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	void LoadFromSaveData(const FFPSavableData& SaveData);
	/** Called after the actor has been loaded from save data */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	void OnLoadedFromSaveData();
};

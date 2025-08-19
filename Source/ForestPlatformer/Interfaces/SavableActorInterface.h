// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/FPSaveGame.h"
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
	virtual void InitializeSaveID();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	FName GetSaveID() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	void SetSaveID(const FName& InSaveID);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	bool ShouldSave() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	FFPSavableData GetSaveData() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	void LoadFromSaveData(const FFPSavableData& SaveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SavableObject")
	void OnLoadedFromSaveData();
};

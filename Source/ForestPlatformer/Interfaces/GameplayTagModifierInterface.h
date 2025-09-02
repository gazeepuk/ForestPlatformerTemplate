// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagModifierInterface.generated.h"

struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE()
class UGameplayTagModifierInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FORESTPLATFORMER_API IGameplayTagModifierInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Adds the specified gameplay tag to a gameplay tag container */
	virtual void AddGameplayTag(const FGameplayTag& InGameplayTag) = 0;
	/** Removes the specified gameplay tag from a gameplay tag container */
	virtual void RemoveGameplayTag(const FGameplayTag& InGameplayTag) = 0;
};

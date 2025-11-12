// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can initiate interactions with other interactable actors.
 * It provides a way to check whether the actor can interact at this point of time.
 */
class FORESTPLATFORMER_API IInteractorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Checks if this actor can interact */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract() const;
};

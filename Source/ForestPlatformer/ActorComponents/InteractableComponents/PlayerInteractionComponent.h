// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"


class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInteractableComponent;

/**
 * Component that handles player interaction with interactable objects in the world
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInteractionComponent();

	/** Binds the interaction input actions */
	UFUNCTION(BlueprintCallable)
	void BindInteractionAction();

	/** Adds an interactable component to the list of available interactables */
	void AddInteractable(UInteractableComponent* InInteractableComponent);
	/** Removes an interactable component from the list of available interactables */
	void RemoveInteractable(UInteractableComponent* InInteractableComponent);

protected:
	/** Returns the closest interactable component from the available list */
	UInteractableComponent* GetClosestInteractable() const;

	/** Updates focused interactabe component */
	void UpdateFocusedInteractable();

	/** Performs the interaction with the current focused interactable */
	void PerformInteraction() const;

	/** Determines if the player can interact with objects */
	bool CanInteract() const;

	/** Array of all interactable components currently within interaction range */
	UPROPERTY()
	TArray<UInteractableComponent*> AvailableInteractables;

	/** The currently focused interactable component */
	TObjectPtr<UInteractableComponent> FocusedInteractable;

	/** Input mapping context containing the interaction input bindings */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Input")
	TObjectPtr<UInputMappingContext> InteractionMappingContext;

	/** Input action that triggers interaction */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Input")
	TObjectPtr<UInputAction> InteractAction;

	/** Priority level for the interaction input mapping context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Input")
	int32 InteractionPriority = 1;

private:
	/** Callback function triggered when the interact input action is started */
	UFUNCTION()
	void InteractAction_Started(const FInputActionValue& InputActionValue);
	/** Timer handle for managing periodic updates of focused interactables */
	FTimerHandle UpdateInteractableTimerHandle;
};

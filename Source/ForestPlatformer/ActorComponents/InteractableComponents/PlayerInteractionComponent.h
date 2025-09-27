// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
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

	/** Set the specified collision for interaction */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Collision")
	void SetInteractionCollision(UShapeComponent* InInteractionCollision);

	UFUNCTION(BlueprintPure)
	AActor* GetFocusedInteractableActor() const;
protected:
	/** Returns the closest interactable component from the available list */
	AActor* GetClosestInteractableActor() const;

	/** Updates focused interactable component */
	void UpdateFocusedInteractableActor();

	/** Performs the interaction with the current focused interactable */
	void PerformInteraction() const;

	/** Determines if the player can interact with objects */
	bool CanInteract() const;

	/** Collision that handles overlapping with interactable actors */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction|Collision")
	TObjectPtr<UShapeComponent> InteractionCollision;

	/** The array of available interactable actors */
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> AvailableInteractableActors;

	/** The currently focused interactable actor */
	TWeakObjectPtr<AActor> FocusedInteractableActor;
	
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
	/** Adds the actor to the available interactable actors array */
	void AddActorToInteractableActor(AActor* InActor);

	/** Removes the actor from the available interactable actors array */
	void RemoveActorInteractableActor(const AActor* InActor);
	
	/** Callback function triggered when an actor enters the interaction collision area */
	UFUNCTION()
	void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Callback function triggered when an actor enters the interaction collision area */
	UFUNCTION()
	void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/** Callback function triggered when the interact input action is started */
	UFUNCTION()
	void InteractAction_Started(const FInputActionValue& InputActionValue);
	
	/** Timer handle for managing periodic updates of focused interactables */
	FTimerHandle UpdateInteractableTimerHandle;

	/** Sets the interactable actor focus state */
	static void SetInteractableActorFocused(AActor* InActor, bool bFocused);
};

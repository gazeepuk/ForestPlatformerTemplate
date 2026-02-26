// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "InteractableActorBase.generated.h"

class UInteractableComponent;
class UWidgetComponent;
class USphereComponent;

/**
 * Base interactable actor class with interactable components and implemented interactable interface.
 */
UCLASS()
class FORESTPLATFORMER_API AInteractableActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AInteractableActorBase();

	// Minimal IInteractableInterface implementation
	virtual UInteractableComponent* GetInteractableComponent_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor* InInstigator) override;

protected:
	virtual void BeginPlay() override;

	// Collision component that handles overlapping with player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Collision")
	TObjectPtr<USphereComponent> InteractableCollision;
	// Widget components that shows interaction indicator widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Widget")
	TObjectPtr<UWidgetComponent> InteractableWidgetComponent;

	// Interactable component that handles collision and widget components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;

private:
	// Indicates whether the actor should initialize component on BeginPlay by default
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bAutoInitInteractableComponent = true;
};

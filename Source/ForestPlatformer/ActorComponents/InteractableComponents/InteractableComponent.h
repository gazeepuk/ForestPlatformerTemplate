// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionDelegate, AActor*, Instigator);

class UWidgetComponent;

/**
 * Component that makes an actor interactable
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	/** Sets interactable collision and widget and setups their properties */
	UFUNCTION(BlueprintCallable)
	void InitInteractableComponent(UShapeComponent* InCollision, UWidgetComponent* InWidgetComponent);
	
	/** Sets the collision that will detect when players enter interaction range */
	UFUNCTION(BlueprintCallable)
	void SetInteractableCollision(UShapeComponent* InCollision);

	/** Sets the widget component that will display if the interactable is focused */
	UFUNCTION(BlueprintCallable)
	void SetInteractionWidgetComponent(UWidgetComponent* InWidgetComponent);

	/** Sets the focus state of this interactable */
	void SetIsFocused(bool bIsFocused);
protected:
	virtual void OnRegister() override;
	/** Collision component used to detect when the player enters interaction range */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> InteractionCollision;

	/** Widget component that displays interaction widget */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

private:
	/** Indicates whether this interactable is currently focused by the player */
	bool bFocused;
};

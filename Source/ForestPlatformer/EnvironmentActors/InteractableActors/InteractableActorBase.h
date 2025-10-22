// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "InteractableActorBase.generated.h"

class UInteractableComponent;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class FORESTPLATFORMER_API AInteractableActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AInteractableActorBase();
	
	virtual UInteractableComponent* GetInteractableComponent_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor* InInstigator) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Collision")
	TObjectPtr<USphereComponent> InteractableCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Widget")
	TObjectPtr<UWidgetComponent> InteractableWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bAutoInitInteractableComponent = true;
};

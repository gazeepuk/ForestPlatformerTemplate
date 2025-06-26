// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"


class UInputMappingContext;
struct FInputActionValue;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Trace")
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Trace")
	float TraceRadius = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Input")
	TObjectPtr<UInputMappingContext> InteractionMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Input")
	int32 InteractionPriority = 1;
private:
	void BindInteractionAction();

	UFUNCTION()
	void InteractAction_Started(const FInputActionValue& InputActionValue);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObservableTriggered, UObservableComponent*, ObservableActor);

/**
 * Component that represents an observable entity which can be triggered
 * and notifies observers when its state cganges to triggered
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UObservableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UObservableComponent();

	/** Checks if this component has been triggered */
	FORCEINLINE bool HasTriggered() const { return bTriggered; }

	/** Delegate that broadcasts when this component is triggered */
	UPROPERTY(BlueprintAssignable)
	FOnObservableTriggered OnObservableTriggered;

	/** Triggers this component and notifies all subscribers */
	UFUNCTION(BlueprintCallable)
	void TriggerObservable();

private:
	/** Indicates whether this component has been triggered */
	bool bTriggered = false;
};

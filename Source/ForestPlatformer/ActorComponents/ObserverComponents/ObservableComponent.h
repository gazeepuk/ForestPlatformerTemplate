// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObservableStateChanged, UObservableComponent*, ObservableActor, bool, bObservableActive);

/**
 * Component that represents an observable entity which can be triggered
 * and notifies observers when its state changes to triggered
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UObservableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UObservableComponent();

	/** Checks if this component has been triggered */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsObservableActive() const { return bObservableActive; }

	/** Delegate that broadcasts when this component is triggered */
	UPROPERTY(BlueprintAssignable)
	FOnObservableStateChanged OnObservableStateChanged;

	/** Triggers this component and notifies all subscribers */
	UFUNCTION(BlueprintCallable)
	void SetObservableActive(bool bNewActive);

private:
	/** Indicates whether this component has been triggered */
	bool bObservableActive = false;
};

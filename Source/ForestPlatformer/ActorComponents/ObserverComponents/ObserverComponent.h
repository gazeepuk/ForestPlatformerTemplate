// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObserverComponent.generated.h"

class UObservableComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllObservablesActivated);

/**
 * Component that observes multiple observable actors and triggers event
 * when all observed actors have been triggered
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UObserverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObserverComponent();

	/** Delegate that broadcasts when all observables are activated at the current time */
	UPROPERTY(BlueprintAssignable)
	FOnAllObservablesActivated OnAllObservablesActivated;

protected:
	virtual void BeginPlay() override;

	/** Subscribes to observable delegates */
	void InitObservables();

	/** Array of actors containing observable component to monitor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observer")
	TArray<AActor*> ObservableActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observer")
	bool bCanTriggerAgain = true;
	
private:
	/** Callback function triggered when an observable component is triggered */
	UFUNCTION()
	void OnObservableStateChanged(UObservableComponent* InObservable, bool bObservableActive);

	/** Checks the observables states. Broadcasts OnAllObservablesActivated, if all the observables are activated. */
	void CheckObservablesState();

	/** Map of the observables and their states */
	TMap<UObservableComponent*, bool> Observables;

	bool bHasTriggered = false;
};

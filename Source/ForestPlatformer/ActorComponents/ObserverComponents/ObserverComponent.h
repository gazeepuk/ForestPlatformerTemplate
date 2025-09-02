// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObserverComponent.generated.h"

class UObservableComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllObservablesTriggered);

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

	/** Delegate that broadcasts when all observables have been triggered */
	UPROPERTY(BlueprintAssignable)
	FOnAllObservablesTriggered OnAllObservablesTriggered;

protected:
	virtual void BeginPlay() override;

	/** Subscribes to observable delegates and initialize the tracking count */
	void InitObservables();

	/** Array of actors containing observable component to monitor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observer")
	TArray<AActor*> ObservableActors;

	/** Callback function triggered when an observable component is triggered */
	UFUNCTION()
	void OnObservableTriggered(UObservableComponent* InObservable);

	/** Count of observables that have not been triggered yet */
	int32 RemainingObservables = 0;
};

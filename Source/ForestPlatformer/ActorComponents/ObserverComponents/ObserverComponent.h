// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObserverComponent.generated.h"

class UObserverCondition;
class UObservableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllObservablesActivated);

USTRUCT(BlueprintType)
struct FObservedCondition
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> ObservableActor = nullptr;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UObserverCondition> ObserverCondition = nullptr;

	bool IsValid() const { return ObservableActor && ObserverCondition; }
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observer")
	bool bCanTriggerAgain = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observer")
	TArray<FObservedCondition> ObservedConditions;
	
private:
	/** Subscribes to observable delegates */
	void InitObservables();
	/** Callback function triggered when an observable component is triggered */
	UFUNCTION()
	void OnObservableStateChanged(UObservableComponent* InChangedObservable);
	/** Checks the observables states. Broadcasts OnAllObservablesActivated, if all the observables are activated. */
	void CheckObservableStates();

	bool bHasTriggered = false;
	
	TMap<UObservableComponent*, UObserverCondition*> ObservableToCondition;
	TArray<FObservedCondition> ObservablesConditions;
};

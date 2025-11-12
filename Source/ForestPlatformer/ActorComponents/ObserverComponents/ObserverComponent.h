// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObservableComponent.h"
#include "Components/ActorComponent.h"
#include "ObserverComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpObserver, Log, All);

class UObserverCondition;
class UObservableComponent;

// Broadcasts when after condition checking
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckConditions, bool, bConditionsMet);
// Broadcasts when all conditions have met
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllConditionsMet);

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

	/** Delegate that broadcasts when checking conditions */
	UPROPERTY(BlueprintAssignable)
	FOnCheckConditions OnCheckConditions;

	/** Delegate that broadcasts when an observable state changed */
	UPROPERTY(BlueprintAssignable)
	FOnObservableStateChanged OnObservableStateChanged;
	
	/** Delegate that broadcasts when all conditions met */
	UPROPERTY(BlueprintAssignable)
	FOnAllConditionsMet OnAllConditionsMet;

	/** Returns true, if all observables have met their conditions */
	UFUNCTION(BlueprintPure, Category = "Observer")
	FORCEINLINE bool HaveConditionsMet() const { return bConditionsMet; }

	/** Resets bHasTriggered so the component can trigger again */
	UFUNCTION(BlueprintCallable, Category = "Observer")
	void ResetHasTrigger();

protected:
	virtual void BeginPlay() override;

	// If the value is true, the component won't trigger again after any observable changes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observer")
	bool bCanTriggerAgain = true;

	// List of all observing actors and their expecting conditions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observer")
	TArray<FObservedCondition> ObservedConditions;

private:
	/** Subscribes to observable delegates */
	void InitObservables();
	/** Callback function triggered when an observable component is triggered */
	UFUNCTION()
	void ObservableStateChanged(UObservableComponent* InChangedObservable);
	/** Checks the observables states. Broadcasts OnAllObservablesActivated, if all the observables are activated. */
	void CheckObservableStates();

	/** Indicates whether the component has successfully triggered */
	bool bHasTriggered = false;

	/** A map of observable components and conditions they must meet*/
	UPROPERTY()
	TMap<UObservableComponent*, UObserverCondition*> ObservableToCondition;

	/** Indicates whether all conditions met or not */
	bool bConditionsMet = false;
};

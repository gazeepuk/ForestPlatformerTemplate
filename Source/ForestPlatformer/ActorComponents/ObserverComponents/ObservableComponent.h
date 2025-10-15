// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableComponent.generated.h"

class UObservableState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObservableStateChanged, UObservableComponent*, ObservableActor);

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
	
	/** Delegate that broadcasts when this component is triggered */
	UPROPERTY(BlueprintAssignable, Category = "Observable")
	FOnObservableStateChanged OnObservableStateChanged;
	
	UFUNCTION(BlueprintPure, Category = "Observable")
	FORCEINLINE UObservableState* GetObservableState() const { return ObservableState; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Observable")
	TObjectPtr<UObservableState> ObservableState;

private:
	UFUNCTION()
	void ObservableStateChanged();
};

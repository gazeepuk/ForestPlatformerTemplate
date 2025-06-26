// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObservableTriggered, UObservableComponent*, ObservableActor);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UObservableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UObservableComponent();

	FORCEINLINE bool HasTriggered() const { return bTriggered; }
	
	UPROPERTY(BlueprintAssignable)
	FOnObservableTriggered OnObservableTriggered;

	UFUNCTION(BlueprintCallable)
	void TriggerObservable();

private:
	bool bTriggered = false;
};

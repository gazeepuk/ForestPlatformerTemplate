// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObserverComponent.generated.h"

class UObservableComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllObservablesTriggered);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UObserverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObserverComponent();
	
	UPROPERTY(BlueprintAssignable)
	FOnAllObservablesTriggered OnAllObservablesTriggered;

protected:
	virtual void BeginPlay() override;

	void InitObservables();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observer")
	TArray<AActor*> ObservableActors;

	UFUNCTION()
	void OnObservableTriggered(UObservableComponent* InObservable);

	int32 RemainingObservables = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "StompBoxComponent.generated.h"

class UArrowComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FStompBoxOverlappedDelegate, UPrimitiveComponent*, OverlappedComp, AActor*,
                                             OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool,
                                             bFromSweep, const FHitResult&, SweepResult);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UStompBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UStompBoxComponent();
	
	UPROPERTY(BlueprintAssignable)
	FStompBoxOverlappedDelegate OnStompBoxOverlappedActorAbove;
	
protected:
	virtual bool IsActorAbove(const AActor* InActor);
	
	UPROPERTY(EditDefaultsOnly)
	double StompHeightBuffer = 10.f;

	TObjectPtr<UArrowComponent> StompDirectionArrow;
private:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

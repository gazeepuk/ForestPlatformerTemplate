// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "WindBoxComponent.generated.h"


class UArrowComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UWindBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UWindBoxComponent();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Lifting")
	float LiftSpeed = 15000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lifting")
	bool bLifting = true;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	TArray<AActor*> OverlappedActors;
};

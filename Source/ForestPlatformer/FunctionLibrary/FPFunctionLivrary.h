// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPFunctionLivrary.generated.h"

class UFPEffectBase;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UFPFunctionLivrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static bool NativeDoesActorHaveTag(AActor* InActor, const FGameplayTag& InGameplayTag);

	static bool NativeTryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass);
	UFUNCTION(BlueprintCallable, DisplayName = "Try Apply Effect By Class To Actor")
	static bool BP_TryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass);

	UFUNCTION(BlueprintPure)
	static bool IsPawnHostile(const APawn* InInstigator, const APawn* InTarget);
};

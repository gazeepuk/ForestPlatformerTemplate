// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreTypes/FPEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPFunctionLibrary.generated.h"

class UFPEffectBase;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UFPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static bool NativeDoesActorHaveTag(AActor* InActor, const FGameplayTag& InGameplayTag);
	UFUNCTION(BlueprintCallable, DisplayName = "Does Actor Have Tag", meta = (ExpandEnumAsExecs = "OutConfirmType"))
	static bool BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InGameplayTag, EFPConfirmType& OutConfirmType);

	UFUNCTION(BlueprintCallable, DisplayName = "Add Gameplay Tag to Actor")
	static void NativeAddGameplayTagToActor(AActor* InActor, FGameplayTag InGameplayTag); 
	UFUNCTION(BlueprintCallable, DisplayName = "Remove Gameplay Tag from Actor")
	static void NativeRemoveGameplayTagFromActor(AActor* InActor, FGameplayTag InGameplayTag); 
	
	static bool NativeTryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass);
	UFUNCTION(BlueprintCallable, DisplayName = "Try Apply Effect By Class To Actor", meta = (ExpandEnumAsExecs = "OutSuccessType"))
	static bool BP_TryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass,
	                                            EFPSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable)
	static void RemoveEffectByClassFromActor(AActor* InActor, TSubclassOf<UFPEffectBase> InEffectClass);
	
	UFUNCTION(BlueprintPure)
	static bool IsPawnHostile(const APawn* InInstigator, const APawn* InTarget);

	UFUNCTION(BlueprintCallable)
	static FName GenerateSaveIDByActorLocation(const AActor* InActor);
};

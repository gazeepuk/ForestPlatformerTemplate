// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreTypes/EnumTypes/FPEnumTypes.h"
#include "CoreTypes/InventoryTypes/InventorySlot.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FPFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFpFunctionLibrary, Log, All);

class UFPEffectBase;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API UFPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Checks if an actor has the specified tag (C++) */
	static bool NativeDoesActorHaveTag(const AActor* InActor, const FGameplayTag& InGameplayTag);
	UFUNCTION(BlueprintCallable, DisplayName = "Does Actor Have Tag", meta = (ExpandEnumAsExecs = "OutConfirmType"))
	/* Checks if an actor has the specified tag (BP) */
	static bool BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InGameplayTag, EFPConfirmType& OutConfirmType);

	/* Adds the specified gameplay tag to the actor */
	UFUNCTION(BlueprintCallable, DisplayName = "Add Gameplay Tag to Actor")
	static void NativeAddGameplayTagToActor(AActor* InActor, FGameplayTag InGameplayTag); 
	/* Removes the specified gameplay from the actor */
	UFUNCTION(BlueprintCallable, DisplayName = "Remove Gameplay Tag from Actor")
	static void NativeRemoveGameplayTagFromActor(AActor* InActor, FGameplayTag InGameplayTag); 

	/* Attempts to apply an effect to an actor by class (C++)*/
	static bool NativeTryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass);
	/* Attempts to apply an effect to an actor by class (BP)*/
	UFUNCTION(BlueprintCallable, DisplayName = "Try Apply Effect By Class To Actor", meta = (ExpandEnumAsExecs = "OutSuccessType"))
	static bool BP_TryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass,
	                                            EFPSuccessType& OutSuccessType);

	/* Removes the effect of the specified class from an actor */
	UFUNCTION(BlueprintCallable)
	static void RemoveEffectByClassFromActor(AActor* InActor, TSubclassOf<UFPEffectBase> InEffectClass);

	/* Determines if one pawn is hostile towards another one */
	UFUNCTION(BlueprintPure)
	static bool IsPawnHostile(const APawn* InInstigator, const APawn* InTarget);

	/* Generates a unique identifier based on actor's location*/
	UFUNCTION(BlueprintCallable)
	static FName GenerateSaveID(const AActor* InActor);

	/* Serializes a structure into a byte array. */
	UFUNCTION(BlueprintCallable, CustomThunk, Category="Serialization", meta=(CustomStructureParam = "Struct"))
	static bool SerializeStruct(const FGenericStruct& Struct, TArray<uint8>& OutBytes);

	DECLARE_FUNCTION(execSerializeStruct);

	/* Deserialize a byte array back into a structure. */
	UFUNCTION(BlueprintCallable, CustomThunk, Category="Serialization", meta=(CustomStructureParam = "Struct"))
	static bool DeserializeStruct(UPARAM(ref) FGenericStruct& Struct, const TArray<uint8>& InBytes);

	DECLARE_FUNCTION(execDeserializeStruct);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (ExpandEnumAsExecs = "OutSuccessType"))
	static void GetItemInfoFromInventorySlotSync(const FInventorySlot& InInventorySlot, FInventoryItem& OutInventoryItemInfo, EFPSuccessType& OutSuccessType);
	
	UFUNCTION(BlueprintPure)
	static int32 LoopArrayIndex(int32 InIndex, int32 InArrayNum);
};

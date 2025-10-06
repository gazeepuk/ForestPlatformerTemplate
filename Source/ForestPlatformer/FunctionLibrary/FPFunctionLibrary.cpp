// Fill out your copyright notice in the Description page of Project Settings.

#include "FPFunctionLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "ActorComponents/EffectComponent/FPEffectComponent.h"
#include "Effects/FPEffectBase.h"
#include "Interfaces/GameplayTagModifierInterface.h"
#include "Interfaces/SavableActorInterface.h"


bool UFPFunctionLibrary::NativeDoesActorHaveTag(const AActor* InActor, const FGameplayTag& InGameplayTag)
{
	if(!InGameplayTag.IsValid() || !InActor)
	{
		return false;
	}
	
	if(const IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(InActor))
	{
		FGameplayTagContainer OwnedGameplayTags;
		GameplayTagAssetInterface->GetOwnedGameplayTags(OwnedGameplayTags);
		return OwnedGameplayTags.HasTagExact(InGameplayTag);
	}
	return false;
}

bool UFPFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InGameplayTag, EFPConfirmType& OutConfirmType)
{
	const bool bHasTag = NativeDoesActorHaveTag(InActor, InGameplayTag);
	OutConfirmType = bHasTag ? EFPConfirmType::Yes : EFPConfirmType::No;
	return bHasTag;
}

void UFPFunctionLibrary::NativeAddGameplayTagToActor(AActor* InActor, FGameplayTag InGameplayTag)
{
	if(!InGameplayTag.IsValid())
	{
		return;
	}
	
	if(IGameplayTagModifierInterface* GameplayTagModifierInterface = Cast<IGameplayTagModifierInterface>(InActor))
	{
		GameplayTagModifierInterface->AddGameplayTag(InGameplayTag);
		UE_LOG(LogTemp, Display, TEXT("Added %s tag to %s"), *InGameplayTag.ToString(), *GetNameSafe(InActor));
	}
}

void UFPFunctionLibrary::NativeRemoveGameplayTagFromActor(AActor* InActor, FGameplayTag InGameplayTag)
{
	if(!InGameplayTag.IsValid())
	{
		return;
	}
	
	if(IGameplayTagModifierInterface* GameplayTagModifierInterface = Cast<IGameplayTagModifierInterface>(InActor))
	{
		GameplayTagModifierInterface->RemoveGameplayTag(InGameplayTag);
		UE_LOG(LogTemp, Display, TEXT("Removed %s tag from %s"), *InGameplayTag.ToString(), *GetNameSafe(InActor));
	}
}

bool UFPFunctionLibrary::NativeTryApplyEffectByClassToActor(AActor* InTargetActor,
                                                            TSubclassOf<UFPEffectBase> InEffectClass)
{
	if(!InTargetActor || !InEffectClass)
	{
		return false;
	}

	if(UFPEffectComponent* EffectComponent = InTargetActor->GetComponentByClass<UFPEffectComponent>())
	{
		UFPEffectBase* EffectToApply = NewObject<UFPEffectBase>(InTargetActor, InEffectClass);

		return EffectComponent->TryApplyEffect(EffectToApply);
	}

	return false;
}

bool UFPFunctionLibrary::BP_TryApplyEffectByClassToActor(AActor* InTargetActor, TSubclassOf<UFPEffectBase> InEffectClass, EFPSuccessType& OutSuccessType)
{
	const bool bAppliedSuccessfully = NativeTryApplyEffectByClassToActor(InTargetActor, InEffectClass);
	OutSuccessType = bAppliedSuccessfully ? EFPSuccessType::Successful : EFPSuccessType::Failed;
	return bAppliedSuccessfully;
}

void UFPFunctionLibrary::RemoveEffectByClassFromActor(AActor* InActor, TSubclassOf<UFPEffectBase> InEffectClass)
{
	if(!InActor || !InEffectClass)
	{
		return;
	}

	if(UFPEffectComponent* EffectComponent = InActor->GetComponentByClass<UFPEffectComponent>())
	{
		EffectComponent->RemoveEffectByClass(InEffectClass);
	}
}

bool UFPFunctionLibrary::IsPawnHostile(const APawn* InInstigator, const APawn* InTarget)
{
	if(!InInstigator || !InTarget)
	{
		return false;
	}
	
	const IGenericTeamAgentInterface* InstigatorTeamAgentInterface = Cast<IGenericTeamAgentInterface>(InInstigator->GetController());
	const IGenericTeamAgentInterface* TargetTeamAgentInterface = Cast<IGenericTeamAgentInterface>(InTarget->GetController());

	if(!InstigatorTeamAgentInterface || !TargetTeamAgentInterface)
	{
		return false;
	}
	
	return InstigatorTeamAgentInterface->GetTeamAttitudeTowards(*InTarget) == ETeamAttitude::Hostile;
}

FName UFPFunctionLibrary::GenerateSaveIDByActorLocation(const AActor* InActor)
{
	FName AutoSaveID;
	if(InActor)
	{
		const FVector Location = InActor->GetActorLocation();
		AutoSaveID = FName(FString::Printf(
			TEXT("%s_%.3f_%.3f_%.3f"),
			*InActor->GetClass()->GetName(),
			Location.X,
			Location.Y,
			Location.Z));
	}
	return AutoSaveID;
}

bool UFPFunctionLibrary::SerializeStruct(const FGenericStruct& Struct, TArray<uint8>& OutBytes)
{
	return false;
}

DEFINE_FUNCTION(UFPFunctionLibrary::execSerializeStruct)
{
	bool& bSuccess = *(bool*)RESULT_PARAM;
	bSuccess = false;

	// Step to take a struct
	Stack.MostRecentProperty = nullptr;
	Stack.Step(Stack.Object, nullptr);
	
	FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* StructData = Stack.MostRecentPropertyAddress;
	if(!StructProperty || !StructData)
	{
		P_FINISH;
		return;
	}
	
	UScriptStruct* StructType = StructProperty->Struct;
	if(!StructType)
	{
		P_FINISH;
		return;
	}

	// Step to take an array
	Stack.Step(Stack.Object, nullptr);
	
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
	void* ArrayPtr = Stack.MostRecentPropertyAddress;
	if(!ArrayProperty || !ArrayPtr)
	{
		P_FINISH;
		return;
	}
	
	TArray<uint8>* OutBytesPtr = (TArray<uint8>*)ArrayPtr;
	OutBytesPtr->Empty();

	// Serialize the struct
	FMemoryWriter MemoryWriter(*OutBytesPtr, true);
	StructType->SerializeBin(MemoryWriter, StructData);

	UE_LOG(LogTemp, Warning, TEXT("SerializeStruct: Serializing struct %s size %d"), *StructType->GetName(), StructType->GetStructureSize());
	UE_LOG(LogTemp, Warning, TEXT("OutBytesPtr: Serialized %d bytes"), OutBytesPtr->Num()); 
	
	bSuccess = true;
	P_FINISH;
}

bool UFPFunctionLibrary::DeserializeStruct(UPARAM(ref) FGenericStruct& Struct, const TArray<uint8>& InBytes)
{
	return false;
}

DEFINE_FUNCTION(UFPFunctionLibrary::execDeserializeStruct)
{
	bool& bSuccess = *(bool*)RESULT_PARAM;
	bSuccess = false;

	// Step to take a struct
	Stack.MostRecentProperty = nullptr;
	Stack.Step(Stack.Object, nullptr);

	FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* StructData = Stack.MostRecentPropertyAddress;
	if(!StructProperty || !StructData)
	{
		P_FINISH;
		return;
	}

	UScriptStruct* StructType = StructProperty->Struct;
	if(!StructType)
	{
		P_FINISH;
		return;
	}

	// Step to take an array
	Stack.Step(Stack.Object, nullptr);
	
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
	void* ArrayPtr = Stack.MostRecentPropertyAddress;
	if(!ArrayProperty || !ArrayPtr)
	{
		P_FINISH;
		return;
	}
	
	TArray<uint8>* InBytesPtr = (TArray<uint8>*)ArrayPtr;
	if(!InBytesPtr || InBytesPtr->IsEmpty())
	{
		P_FINISH;
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("InBytes: num %d bytes"), InBytesPtr->Num());

	// Deserialize the array
	StructType->InitializeStruct(StructData);
	FMemoryReader MemoryReader(*InBytesPtr, true);
	StructType->SerializeBin(MemoryReader, StructData);
	
	UE_LOG(LogTemp, Warning, TEXT("Deserialized struct %s from %d bytes"), *StructType->GetName(), InBytesPtr->Num());
	bSuccess = true;

	P_FINISH;
}

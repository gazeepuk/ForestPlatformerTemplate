// Fill out your copyright notice in the Description page of Project Settings.

#include "FPFunctionLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "ActorComponents/EffectComponent/FPEffectComponent.h"
#include "Effects/FPEffectBase.h"
#include "Interfaces/GameplayTagModifierInterface.h"

bool UFPFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, const FGameplayTag& InGameplayTag)
{
	if(!InGameplayTag.IsValid() || !InActor)
	{
		return false;
	}
	
	if(IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(InActor))
	{
		FGameplayTagContainer OwnedGameplayTags;
		GameplayTagAssetInterface->GetOwnedGameplayTags(OwnedGameplayTags);
		return OwnedGameplayTags.HasTagExact(InGameplayTag);
	}
	return false;
}

bool UFPFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InGameplayTag,
                                             EFPConfirmType& OutConfirmType)
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

bool UFPFunctionLibrary::BP_TryApplyEffectByClassToActor(AActor* InTargetActor,
                                                         TSubclassOf<UFPEffectBase> InEffectClass,
                                                         EFPSuccessType& OutSuccessType)
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

// Fill out your copyright notice in the Description page of Project Settings.

#include "FPFunctionLivrary.h"

#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "ActorComponents/EffectComponent/FPEffectComponent.h"
#include "Effects/FPEffectBase.h"

bool UFPFunctionLivrary::NativeDoesActorHaveTag(AActor* InActor, const FGameplayTag& InGameplayTag)
{
	if(IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(InActor))
	{
		FGameplayTagContainer OwnedGameplayTags;
		GameplayTagAssetInterface->GetOwnedGameplayTags(OwnedGameplayTags);
		return OwnedGameplayTags.HasTagExact(InGameplayTag);
	}
	return false;
}

bool UFPFunctionLivrary::NativeTryApplyEffectByClassToActor(AActor* InTargetActor,
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

bool UFPFunctionLivrary::BP_TryApplyEffectByClassToActor(AActor* InTargetActor,
	TSubclassOf<UFPEffectBase> InEffectClass)
{
	return NativeTryApplyEffectByClassToActor(InTargetActor, InEffectClass);
}

bool UFPFunctionLivrary::IsPawnHostile(const APawn* InInstigator, const APawn* InTarget)
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
	
	return InstigatorTeamAgentInterface->GetGenericTeamId() == TargetTeamAgentInterface->GetGenericTeamId();
}

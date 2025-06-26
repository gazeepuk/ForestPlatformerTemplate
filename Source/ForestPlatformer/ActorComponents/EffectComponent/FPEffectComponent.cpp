// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEffectComponent.h"

#include "Effects/FPEffectBase.h"


UFPEffectComponent::UFPEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UFPEffectComponent::TryApplyEffect(UFPEffectBase* Effect)
{
	if (!Effect || !Effect->CanApply(GetOwner()))
	{
		return false;
	}

	// Check if already applied
	if (ActiveEffects.ContainsByPredicate([Effect](const FFPActiveEffect& Active){ return Active.Effect->GetEffectID() == Effect->GetEffectID(); }))
	{
		return false;
	}

	FFPActiveEffect NewEffect;
	NewEffect.Effect = Effect;
    
	if (Effect->GetDurationType() == EFPEffectDurationType::Duration)
	{
		NewEffect.RemainingTime = Effect->GetDuration();
		GetWorld()->GetTimerManager().SetTimer(
			NewEffect.TimerHandle,
			[this, Effect](){ OnEffectExpired(Effect); },
			Effect->GetDuration(),
			false
		);
	}

	Effect->OnApplied(GetOwner());

	if(Effect->GetDurationType() != EFPEffectDurationType::Instant)
	{
		ActiveEffects.Add(NewEffect);
	}
	
	return true;
}

void UFPEffectComponent::RemoveEffect(const UFPEffectBase* Effect)
{
	int32 EffectIndex = ActiveEffects.IndexOfByPredicate([Effect](const FFPActiveEffect& Active)
	{
		return Active.Effect == Effect;
	});
    
	if (EffectIndex != INDEX_NONE)
	{
		FFPActiveEffect& ActiveEffect = ActiveEffects[EffectIndex];

		GetWorld()->GetTimerManager().ClearTimer(ActiveEffect.TimerHandle);

		ActiveEffect.Effect->OnRemoved(GetOwner());

		ActiveEffects.RemoveAt(EffectIndex);
	}
}

void UFPEffectComponent::OnEffectExpired(const UFPEffectBase* Effect)
{
	RemoveEffect(Effect);
}

void UFPEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FFPActiveEffect& ActiveEffect : ActiveEffects)
	{
		if (ActiveEffect.Effect->GetDurationType() == EFPEffectDurationType::Duration)
		{
			ActiveEffect.RemainingTime -= DeltaTime;
		}
		ActiveEffect.Effect->OnTick(GetOwner(), DeltaTime);
	}
}

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
	FFPActiveEffect* ExistingEffect = ActiveEffects.FindByPredicate([Effect](const FFPActiveEffect& Active)
	{
		return Active.Effect->GetEffectID() == Effect->GetEffectID();
	});
	
	if (ExistingEffect && ExistingEffect->IsValid())
	{
		switch(ExistingEffect->Effect->GetStackingPolicy())
		{
		case EFPEffectStackingPolicy::NoStack:
			return true;
			
		case EFPEffectStackingPolicy::RefreshDuration:
			
			GetWorld()->GetTimerManager().ClearTimer(ExistingEffect->TimerHandle);

			GetWorld()->GetTimerManager().SetTimer(ExistingEffect->TimerHandle,
			[this, ExistingEffect](){ OnEffectExpired(ExistingEffect->Effect); },
			Effect->GetDuration(),
			false);
			
			return true;
			
		case EFPEffectStackingPolicy::AddToRemainingTime:
			const float RemainingTimerDuration = GetWorld()->GetTimerManager().GetTimerRemaining(ExistingEffect->TimerHandle);

			GetWorld()->GetTimerManager().ClearTimer(ExistingEffect->TimerHandle);

			GetWorld()->GetTimerManager().SetTimer(ExistingEffect->TimerHandle,
			[this, ExistingEffect](){ OnEffectExpired(ExistingEffect->Effect); },
			Effect->GetDuration() + RemainingTimerDuration,
			false);
			
			return true;;
		}
	}
	
	FFPActiveEffect NewEffect;
	NewEffect.Effect = Effect;
	
	if (Effect->GetDurationType() == EFPEffectDurationType::Duration)
	{
		NewEffect.EffectDuration = Effect->GetDuration();
		GetWorld()->GetTimerManager().SetTimer(
			NewEffect.TimerHandle,
			[this, Effect](){ OnEffectExpired(Effect); },
			Effect->GetDuration(),
			false
		);
	}

	if(Effect->GetDurationType() != EFPEffectDurationType::Instant)
	{
		ActiveEffects.Add(NewEffect);
	}
	
	Effect->SetWorldContextObject(GetOwner());
	Effect->OnApplied(GetOwner());
	
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
			ActiveEffect.EffectDuration -= DeltaTime;
		}
		ActiveEffect.Effect->OnTick(GetOwner(), DeltaTime);
	}
}

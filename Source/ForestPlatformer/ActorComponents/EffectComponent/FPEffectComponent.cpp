// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEffectComponent.h"
#include "CoreTypes/Effects/FPEffectBase.h"


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

	bool bShouldApplyEffect = false;
	HandleStacking(Effect, bShouldApplyEffect);

	if(!bShouldApplyEffect)
	{
		return true;
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
		return Active.Effect->GetEffectID() == Effect->GetEffectID();
	});
    
	if (EffectIndex != INDEX_NONE)
	{
		FFPActiveEffect& ActiveEffect = ActiveEffects[EffectIndex];

		GetWorld()->GetTimerManager().ClearTimer(ActiveEffect.TimerHandle);

		ActiveEffect.Effect->OnRemoved(GetOwner());

		ActiveEffects.RemoveAt(EffectIndex);
	}
}

void UFPEffectComponent::RemoveEffectByClass(const TSubclassOf<UFPEffectBase> InEffectClass)
{
	if(InEffectClass)
	{
		RemoveEffect(InEffectClass.GetDefaultObject());
	}
}

void UFPEffectComponent::OnEffectExpired(const UFPEffectBase* Effect)
{
	RemoveEffect(Effect);
}

void UFPEffectComponent::HandleStacking(const UFPEffectBase* Effect, bool& bOutShouldApplyEffect)
{
	FFPActiveEffect* ExistingEffect = ActiveEffects.FindByPredicate([Effect](const FFPActiveEffect& Active)
	{
		return Active.Effect->GetEffectID() == Effect->GetEffectID();
	});
	
	if(Effect->GetDurationType() == EFPEffectDurationType::Instant || !ExistingEffect || !ExistingEffect->IsValid())
	{
		bOutShouldApplyEffect = true;
		return;
	}
	
	switch(ExistingEffect->Effect->GetStackingPolicy())
	{
	case EFPEffectStackingPolicy::NoStack:
		bOutShouldApplyEffect = false;
		break;
	case EFPEffectStackingPolicy::ApplyAsSeparatedEffect:
		bOutShouldApplyEffect = true;
		break;
	case EFPEffectStackingPolicy::RefreshDuration:
		bOutShouldApplyEffect = false;
		if(ExistingEffect->Effect->GetDurationType() == EFPEffectDurationType::Duration)
		{
			GetWorld()->GetTimerManager().ClearTimer(ExistingEffect->TimerHandle);

			ExistingEffect->RemainingTime = Effect->GetDuration();
			GetWorld()->GetTimerManager().SetTimer(ExistingEffect->TimerHandle,
			[this, ExistingEffect](){ OnEffectExpired(ExistingEffect->Effect); },
			ExistingEffect->RemainingTime,
			false);
		}
		break;
	case EFPEffectStackingPolicy::AddToRemainingTime:
		bOutShouldApplyEffect = false;
		if(ExistingEffect->Effect->GetDurationType() == EFPEffectDurationType::Duration)
		{
			const float RemainingTimerDuration = ExistingEffect->RemainingTime;
			ExistingEffect->RemainingTime = Effect->GetDuration() + RemainingTimerDuration;
			
			GetWorld()->GetTimerManager().ClearTimer(ExistingEffect->TimerHandle);
			
			GetWorld()->GetTimerManager().SetTimer(ExistingEffect->TimerHandle,
			[this, ExistingEffect](){ OnEffectExpired(ExistingEffect->Effect); },
			ExistingEffect->RemainingTime,
			false);
		}
		break;
	}
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

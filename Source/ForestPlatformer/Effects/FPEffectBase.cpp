// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEffectBase.h"


bool UFPEffectBase::CanApply_Implementation(AActor* Target) const
{
	return true;
}


void UFPEffectBase::OnApplied_Implementation(AActor* Target)
{
}

void UFPEffectBase::OnRemoved_Implementation(AActor* Target)
{
}

void UFPEffectBase::OnTick_Implementation(AActor* Target, float DeltaTime)
{
}

void UFPEffectBase::PostInitProperties()
{
	UObject::PostInitProperties();

	EffectIDHash = GetTypeHash(EffectID);
    
#if WITH_EDITOR
	if (EffectID.IsNone())
	{
		EffectID = *GetClass()->GetName();
		EffectIDHash = GetTypeHash(EffectID);
	}
#endif
}

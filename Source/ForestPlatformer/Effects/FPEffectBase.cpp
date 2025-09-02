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

#if WITH_EDITOR
void UFPEffectBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	if(EffectID.IsNone())
	{
		EffectID = *GetClass()->GetName();
		EffectIDHash = GetTypeHash(EffectID);
	}
	
	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, EffectID))
	{
		if(EffectID.IsNone())
		{
			EffectID = *GetClass()->GetName();
		}
		
		EffectIDHash = GetTypeHash(EffectID);
	}
}
#endif
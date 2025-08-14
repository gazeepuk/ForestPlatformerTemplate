// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"


void UHealthComponent::SetCurrentHealth(const float InNewCurrentHealth)
{
	const float NewCurrentHealth = FMath::Clamp(InNewCurrentHealth, 0, MaxHealth);
	const float OldCurrentHealth = CurrentHealth;
	CurrentHealth = NewCurrentHealth;

	if(CurrentHealth == 0.f)
	{
		UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Dead);
	}
	
	if(NewCurrentHealth != OldCurrentHealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	}
	if(NewCurrentHealth < OldCurrentHealth)
	{
		OnTakeDamage.Broadcast(OldCurrentHealth - CurrentHealth);
	}
	if(NewCurrentHealth == 0.f && OldCurrentHealth != 0.f)
	{
		OnZeroHealth.Broadcast();
	}
}

void UHealthComponent::RestoreCurrentHealth(const float InHealthAmount)
{
	SetCurrentHealth(CurrentHealth + InHealthAmount);
}

void UHealthComponent::SpendCurrentHealth(const float InHealthAmount)
{
	SetCurrentHealth(CurrentHealth - InHealthAmount);
}

void UHealthComponent::TakeDamage(AActor* DamageCauser, float InDamageValue, AController* InstigatedBy)
{
	if(!UFPFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), FPGameplayTags::Shared_Status_Invincible) &&
		!UFPFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), FPGameplayTags::Shared_Status_Dead))
	{
		SpendCurrentHealth(InDamageValue);
	}
}

void UHealthComponent::PostInitProperties()
{
	Super::PostInitProperties();

	CurrentHealth = MaxHealth;
}


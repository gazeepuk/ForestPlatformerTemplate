// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"


bool UHealthComponent::IsAlive() const
{
	// Checks if the owner does not have dead status tag and current health is greater than zero  
	return !UFPFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), FPGameplayTags::Shared_Status_Dead) && CurrentHealth > 0.f;
}

void UHealthComponent::SetMaxHealth(float InNewMaxHealth)
{
	// Clamps new max health and broadcasts changes
	const float NewMaxHealth = FMath::Max(InNewMaxHealth, 1.f);
	MaxHealth = NewMaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::SetCurrentHealth(const float InNewCurrentHealth)
{
	// Clamps new current health
	const float NewCurrentHealth = FMath::Clamp(InNewCurrentHealth, 0, MaxHealth);
	const float OldCurrentHealth = CurrentHealth;
	CurrentHealth = NewCurrentHealth;

	// Adds dead tag status if health is zero
	if(CurrentHealth == 0.f)
	{
		UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Dead);
	}

	// Broadcasts new health
	if(NewCurrentHealth != OldCurrentHealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	}
	// Broadcasts damage value
	if(NewCurrentHealth < OldCurrentHealth && !bLoadingHealth)
	{
		OnTakeDamage.Broadcast(OldCurrentHealth - CurrentHealth);
	}
	// Broadcasts death
	if(NewCurrentHealth == 0.f && OldCurrentHealth != 0.f)
	{
		OnZeroHealth.Broadcast();
	}
}

void UHealthComponent::LoadHealth(float InNewCurrentHealth, float InMaxHealth)
{
	// Loads health. Prevents broadcasting TakeDamage delegate, if the loaded health is less than current health
	bLoadingHealth = true;
	SetMaxHealth(InMaxHealth);
	SetCurrentHealth(InNewCurrentHealth);
	bLoadingHealth = false;
}

void UHealthComponent::RestoreCurrentHealth(const float InHealthAmount)
{
	SetCurrentHealth(CurrentHealth + InHealthAmount);
}

void UHealthComponent::SpendCurrentHealth(const float InHealthAmount)
{
	SetCurrentHealth(CurrentHealth - InHealthAmount);
}

void UHealthComponent::TakeDamage_Implementation(AActor* DamageCauser, float InDamageValue, AController* InstigatedBy)
{
	// Checks if can take damage and takes it.
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


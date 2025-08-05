// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"


void UHealthComponent::SetCurrentHealth(const float InNewCurrentHealth)
{
	const float NewCurrentHealth = FMath::Clamp(InNewCurrentHealth, 0, MaxHealth);
	CurrentHealth = NewCurrentHealth;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	
	if(CurrentHealth == 0)
	{
		UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Dead);
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
		OnTakeDamage.Broadcast(InDamageValue);
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

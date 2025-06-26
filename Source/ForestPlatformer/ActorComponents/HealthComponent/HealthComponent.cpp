// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLivrary.h"


void UHealthComponent::SetCurrentHealth(const float InNewCurrentHealth)
{
	const float NewCurrentHealth = FMath::Clamp(InNewCurrentHealth, 0, MaxHealth);
	CurrentHealth = NewCurrentHealth;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	
	if(CurrentHealth == 0)
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

void UHealthComponent::TakeDamage(AActor* DamageCauser, float InDamage, AController* InstigatedBy)
{
	if(!UFPFunctionLivrary::NativeDoesActorHaveTag(GetOwner(), FPGameplayTags::Shared_Status_Invincible))
	{
		SpendCurrentHealth(InDamage);
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

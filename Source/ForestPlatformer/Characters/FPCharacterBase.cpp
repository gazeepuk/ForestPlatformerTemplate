// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacterBase.h"

#include "ActorComponents/HealthComponent/HealthComponent.h"

AFPCharacterBase::AFPCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AFPCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = OwnedTags;
}

void AFPCharacterBase::TakeDamage_Implementation(AActor* DamageCauser, float InDamage,
	AController* InstigatorController)
{
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageCauser, InDamage, InstigatorController);
	}
}

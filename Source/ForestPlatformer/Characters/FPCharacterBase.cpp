// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacterBase.h"

#include "ActorComponents/HealthComponent/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreTypes/FPCustomCollisions.h"

AFPCharacterBase::AFPCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	GetMesh()->SetCollisionResponseToChannel(ECC_FP_Projectile_OC, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_FP_Projectile_OC, ECR_Overlap);
}

void AFPCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = OwnedTags;
}

void AFPCharacterBase::AddGameplayTag(const FGameplayTag& InGameplayTag)
{
	if(InGameplayTag.IsValid())
	{
		OwnedTags.AddTag(InGameplayTag);
	}
}

void AFPCharacterBase::RemoveGameplayTag(const FGameplayTag& InGameplayTag)
{
	if(InGameplayTag.IsValid())
	{
		OwnedTags.RemoveTag(InGameplayTag);
	}
}

void AFPCharacterBase::TakeDamage_Implementation(AActor* DamageCauser, float InDamage,
                                                 AController* InstigatorController)
{
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageCauser, InDamage, InstigatorController);
	}
}

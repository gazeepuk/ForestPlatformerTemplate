// Developed by Ivan Piankouski. All Rights Reserved


#include "FPCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "CoreTypes/CustomCollisions/FPCustomCollisions.h"

AFPCharacterBase::AFPCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetMesh()->SetCollisionResponseToChannel(ECC_FP_Projectile_OC, ECR_Ignore);
	GetMesh()->bReceivesDecals = false;
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

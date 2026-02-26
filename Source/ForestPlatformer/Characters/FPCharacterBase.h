// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/GameplayTagModifierInterface.h"
#include "FPCharacterBase.generated.h"

class UHealthComponent;

UCLASS()
class FORESTPLATFORMER_API AFPCharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IGameplayTagModifierInterface
{
	GENERATED_BODY()

public:
	AFPCharacterBase(const FObjectInitializer& ObjectInitializer);

	//~Begin IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	//~End IGameplayTagAssetInterface

	//~Begin IGameplayTagModifierInterface
	virtual void AddGameplayTag(const FGameplayTag& InGameplayTag) override;
	virtual void RemoveGameplayTag(const FGameplayTag& InGameplayTag) override;
	//~End IGameplayTagModifierInterface

private:
	FGameplayTagContainer OwnedTags;
};

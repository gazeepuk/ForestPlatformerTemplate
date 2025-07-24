// Fill out your copyright notice in the Description page of Project Settings.

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

	// IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	//IGameplayTagModifierInterface
	virtual void AddGameplayTag(const FGameplayTag& InGameplayTag) override;
	virtual void RemoveGameplayTag(const FGameplayTag& InGameplayTag) override;

private:
	FGameplayTagContainer OwnedTags;
};

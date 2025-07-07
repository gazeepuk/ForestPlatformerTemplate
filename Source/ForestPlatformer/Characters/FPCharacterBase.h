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
class FORESTPLATFORMER_API AFPCharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IDamageableInterface, public IGameplayTagModifierInterface
{
	GENERATED_BODY()

public:
	AFPCharacterBase(const FObjectInitializer& ObjectInitializer);

	// IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	//IGameplayTagModifierInterface
	virtual void AddGameplayTag(const FGameplayTag& InGameplayTag) override;
	virtual void RemoveGameplayTag(const FGameplayTag& InGameplayTag) override;

	//IDamageableInterface
	virtual void TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatorController) override;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;

private:
	FGameplayTagContainer OwnedTags;
};

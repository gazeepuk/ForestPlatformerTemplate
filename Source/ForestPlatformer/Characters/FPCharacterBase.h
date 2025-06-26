// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "FPCharacterBase.generated.h"

class UHealthComponent;

UCLASS()
class FORESTPLATFORMER_API AFPCharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IDamageableInterface
{
	GENERATED_BODY()

public:
	AFPCharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual void TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatorController) override;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;

private:
	FGameplayTagContainer OwnedTags;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponentBase.h"
#include "Components/ActorComponent.h"
#include "PlayerCombatComponent.generated.h"


struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UFPAttackType;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPlayerCombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()

public:
	UPlayerCombatComponent();
	
	virtual bool CanAttack() const override;
	bool TryCurrentAttack();

	UFUNCTION(BlueprintCallable)
	void SetPrimaryAttackByClass(TSubclassOf<UFPAttackType> InPrimaryAttackClass);

	UFUNCTION(BlueprintCallable)
	void ResetPrimaryAttack();
	UFUNCTION(BlueprintCallable)
	void InitCombatComponent();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Input")
	TObjectPtr<UInputMappingContext> CombatMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Input")
	int32 CombatInputPriority = 2;
	
	UFUNCTION()
	void AttackAction_Started(const FInputActionValue& ActionValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UFPAttackType> DefaultAttackType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPAttackType> CurrentPrimaryAttack;

private:
	void BindCombatAction();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPAttackType.generated.h"

class UCombatComponentBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FORESTPLATFORMER_API UFPAttackType : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetAttackTypeID() const {return AttackTypeID;}

	FORCEINLINE uint32 GetAttackTypeIDHash() const {return AttackTypeIDHash;}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FGameplayTag GetAttackTypeTag() const {return AttackTypeTag;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetOwningActor() const { return OwningActor; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCombatComponent(UCombatComponentBase* InCombatComponent) { CombatComponent = InCombatComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCombatComponentBase* GetCombatComponent() const { return CombatComponent; }

	UFUNCTION(BlueprintPure)
	ACharacter* GetCharacterFromOwningActor() const;

	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetSkeletalMeshFromOwningActor() const;

	UFUNCTION(BlueprintCallable)
	virtual void InitAttack(AActor* InOwningActor, UCombatComponentBase* InCombatComponent);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformAttack(AActor* InTargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAttack(AActor* InInstigator);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndAttack();
	
	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;
	
protected:
	virtual void PostInitProperties() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<AActor*> FindTarget();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackTypeTag;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FName AttackTypeID;
	
	uint32 AttackTypeIDHash;

	UPROPERTY()
	TObjectPtr<AActor> OwningActor;

	UPROPERTY()
	TObjectPtr<UCombatComponentBase> CombatComponent;
};

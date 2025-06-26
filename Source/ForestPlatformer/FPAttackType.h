// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPAttackType.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FORESTPLATFORMER_API UFPAttackType : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetAttackTypeID() const {return AttackTypeID;}

	FORCEINLINE uint32 GetAttackTypeIDHash() const {return AttackTypeIDHash;}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FGameplayTag GetAttackTypeTag() const {return AttackTypeTag;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetOwningActor() const { return OwningActor; }
	
	UFUNCTION(BlueprintPure)
	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformAttack(AActor* InTargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAttack(AActor* InInstigator);
	
	FOnMontageEnded OnAttackMontageEndDelegate;
	
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
	
	bool TryPlayAttackAnimation();
};

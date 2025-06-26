// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPCharacterBase.h"
#include "InputActionValue.h"
#include "Interfaces/CoinsWalletInterface.h"
#include "FPPlayerCharacter.generated.h"

class UFPEffectComponent;
class UPlayerCombatComponent;
class UPlayerInteractionComponent;
class UCoinsWalletComponent;
class UFPSpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class FORESTPLATFORMER_API AFPPlayerCharacter : public AFPCharacterBase, public ICoinsWalletInterface
{
	GENERATED_BODY()

public:
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	//~Begin ICoinsWalletInterface
	virtual void AddCoins_Implementation(int32 InCoinsNum) override;
	virtual bool TrySpendCoins_Implementation(int32 InCoinsNum) override;
	virtual bool HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const override;
	virtual void SetCurrentCoins_Implementation(int32 InNewCurrentCoins) override;
	virtual int32 GetCurrentCoins_Implementation() const override;
	//~End ICoinsWalletInterface

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPSpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerInteractionComponent> PlayerInteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPEffectComponent> EffectComponent;
private:
	virtual void Landed(const FHitResult& Hit) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> ZoomCameraAction;
	
	void MoveAction_Triggered(const FInputActionValue& InputActionValue);
	
	void LookAction_Triggered(const FInputActionValue& InputActionValue); 
	
	void JumpAction_Started(const FInputActionValue& InputActionValue);
	void JumpAction_Triggered(const FInputActionValue& InputActionValue);
	void JumpAction_Completed(const FInputActionValue& InputActionValue);
	
	void ZoomCameraAction_Started(const FInputActionValue& InputActionValue);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPCharacterBase.h"
#include "InputActionValue.h"
#include "Interfaces/CoinsWalletInterface.h"
#include "Interfaces/InteractorInterface.h"
#include "FPPlayerCharacter.generated.h"

class UPlayerInteractionComponent;
class UFPEffectComponent;
class UPlayerCombatComponent;
class UCoinsWalletComponent;
class UFPSpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class FORESTPLATFORMER_API AFPPlayerCharacter : public AFPCharacterBase, public ICoinsWalletInterface, public IDamageableInterface, public IInteractorInterface
{
	GENERATED_BODY()

public:
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	//~Begin ICoinsWalletInterface
	/** Add specified  */
	/** Adds the specified number of coins to the player's wallet */
	virtual void AddCoins_Implementation(int32 InCoinsNum) override;
	/** Attempts to spend the specified number of coins from the player's wallet */
	virtual bool TrySpendCoins_Implementation(int32 InCoinsNum) override;
	/** Check if the player has at least the specified number of coins */
	virtual bool HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const override;
	/** Directly sets the specified number of coins in the player's wallet */
	virtual void SetCurrentCoins_Implementation(int32 InNewCurrentCoins) override;
	/** Returns the current number of coins in the player's wallet */
	virtual int32 GetCurrentCoins_Implementation() const override;
	//~End ICoinsWalletInterface

	//~Begin IDamageableInterface
	/**
	 * Applies damage to the player character
	 * @param DamageCauser The actor that caused the damage
	 * @param InDamage The amount of damage to apply
	 * @param InstigatedBy The controller responsible for the damage
	 */
	virtual void TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatedBy) override;
	//~End IDamageableInterface

	//~Begin IInteractorInterface
	/** Determines if the player can currently interact */
	virtual bool CanInteract_Implementation() const override;
	//~End IInteractorInterface
	
protected:
	/** Called when the character is possessed by a controller */
	virtual void PossessedBy(AController* NewController) override;
	/** Sets up player input bindings and mapping */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Handles player's death */
	UFUNCTION(BlueprintNativeEvent)
	void OnDeath();

	/** Callback function called whenever the character takes damage */
	UFUNCTION(BlueprintNativeEvent)
	void OnTakeDamage(float DamageValue);
	
	/** Spring arm component that controls the camera positioning */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPSpringArmComponent> CameraBoom;

	/** Main camera component that provides the player's view perspective */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Component responsible for handling player interaction with interactable objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerInteractionComponent> PlayerInteractionComponentV2;

	/** Component that manages player combat attacks */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerCombatComponent> CombatComponent;

	/** Component that manages effects applied to the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPEffectComponent> EffectComponent;

	/** Component that manages the player's health handling */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;
	
private:
	/** Called when the character lands */
	virtual void Landed(const FHitResult& Hit) override;

	/** Default mapping context for player controls */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Input action for character movement */
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> MoveAction;
	/** Input action for camera rotation */
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> LookAction;
	/** Input action for character jumping */
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> JumpAction;
	/** Input action for camera zoom functionality */
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputActions")
	TObjectPtr<UInputAction> ZoomCameraAction;

	/** Handler for movement input events */
	void MoveAction_Triggered(const FInputActionValue& InputActionValue);
	
	/** Handler for camera rotation input events */
	void LookAction_Triggered(const FInputActionValue& InputActionValue); 
	
	/** Handler for character jumping input started events */
	void JumpAction_Started(const FInputActionValue& InputActionValue);
	/** Handler for character jumping input triggered events */
	void JumpAction_Triggered(const FInputActionValue& InputActionValue);
	/** Handler for character jumping input completed events */
	void JumpAction_Completed(const FInputActionValue& InputActionValue);

	/** handler for camera zoom input started events */
	void ZoomCameraAction_Started(const FInputActionValue& InputActionValue);
	void ZoomCameraAction_Completed(const FInputActionValue& InputActionValue);
	void ZoomCameraAction_Triggered(const FInputActionValue& InputActionValue);
};

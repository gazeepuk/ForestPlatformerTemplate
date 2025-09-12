// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AttackTypes/FPAttackType.h"

bool UPlayerCombatComponent::CanAttack_Implementation()
{
	if(UFPAttackType* CurrentAttack = GetCurrentAttackType())
	{
		return Super::CanAttack_Implementation() && CurrentAttack->CanAttack();
	}
	return false;
}

bool UPlayerCombatComponent::TryPerformCurrentAttack()
{
	return TryActivateAttackByTag(CurrentAttackTag);
}

void UPlayerCombatComponent::SetCurrentAttackByClass(TSubclassOf<UFPAttackType> InPrimaryAttackClass)
{
	UFPAttackType* CurrentAttack = GetCurrentAttackType();

	if(CurrentAttack)
	{
		if(CurrentAttack->GetClass() == InPrimaryAttackClass)
		{
			return;
		}

		if(CurrentAttack->IsActive())
		{
			CurrentAttack->TryAbortAttack();
		}
	}

	CurrentAttack = FindAttackTypeByClass(InPrimaryAttackClass);
	if(CurrentAttack)
	{
		CurrentAttackTag = CurrentAttack->GetAttackTypeTag();
	}
}

FGameplayTag UPlayerCombatComponent::GetCurrentAttackTag() const
{
	return CurrentAttackTag;
}

void UPlayerCombatComponent::SetCurrentAttackByTag(FGameplayTag InAttackTag)
{
	UFPAttackType* CurrentAttack = GetCurrentAttackType();

	if(CurrentAttack)
	{
		if(CurrentAttack->GetAttackTypeTag() == InAttackTag)
		{
			return;
		}

		if(CurrentAttack->IsActive())
		{
			CurrentAttack->EndAttack();
		}
	}

	CurrentAttack = FindAttackTypeByTag(InAttackTag);
	if(CurrentAttack)
	{
		CurrentAttackTag = CurrentAttack->GetAttackTypeTag();
	}
}

void UPlayerCombatComponent::ResetPrimaryAttack()
{
	if(DefaultAttackType)
	{
		SetCurrentAttackByClass(DefaultAttackType);
	}
}

void UPlayerCombatComponent::InitCombatComponent()
{
	BindCombatInput();
	InitAttacks();
	SetCurrentAttackByClass(DefaultAttackType);
}

void UPlayerCombatComponent::UnbindCombatInput()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if(!OwningPawn)
	{
		return;
	}

	APlayerController* PlayerController = OwningPawn->GetController<APlayerController>();
	if(!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if(InputSubsystem)
	{
		InputSubsystem->RemoveMappingContext(CombatMappingContext);
	}
}

UFPAttackType* UPlayerCombatComponent::GetCurrentAttackType()
{
	if(CurrentAttackTag.IsValid())
	{
		return FindAttackTypeByTag(CurrentAttackTag);
	}
	return nullptr;
}

void UPlayerCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindCombatInput();
	Super::EndPlay(EndPlayReason);
}

void UPlayerCombatComponent::AttackAction_Started(const FInputActionValue& ActionValue)
{
	ExecuteAttack();
}

void UPlayerCombatComponent::ExecuteAttack_Implementation()
{
	TryPerformCurrentAttack();
}

void UPlayerCombatComponent::BindCombatInput()
{
	
	APawn* OwningPawn = GetOwner<APawn>();
	if(!OwningPawn)
	{
		return;
	}
	
	if(!AttackAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackAction of UPlayerCombatComponent of %s is not assigned. Binding is termiated."), *GetNameSafe(OwningPawn));
		return;
	}

	APlayerController* PlayerController = OwningPawn->GetController<APlayerController>();
	if(!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	
	if(InputSubsystem && EnhancedInputComponent)
	{
		InputSubsystem->AddMappingContext(CombatMappingContext, CombatInputPriority);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AttackAction_Started);
	}
}

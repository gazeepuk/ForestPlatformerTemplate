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
	SetCurrentAttack(FindAttackTypeByClass(InPrimaryAttackClass));
}

FGameplayTag UPlayerCombatComponent::GetCurrentAttackTag() const
{
	return CurrentAttackTag;
}

void UPlayerCombatComponent::SetCurrentAttackByTag(FGameplayTag InAttackTag)
{
	SetCurrentAttack(FindAttackTypeByTag(InAttackTag));
}

void UPlayerCombatComponent::ResetDefaultAttack()
{
	if(UFPAttackType* DefaultAttack = FindAttackTypeByClass(DefaultAttackType))
	{
		SetCurrentAttack(DefaultAttack);
		return;
	}

	GrantPlayerAttackTypeByClass(DefaultAttackType, true);

	UE_LOG(LogTemp, Warning, TEXT("Can't reset the default attack for %s. Default attack type class is invalid"), *GetNameSafe(GetOwner()))
}

void UPlayerCombatComponent::InitCombatComponent()
{
	BindCombatInput();
	InitAttacks();
	ResetDefaultAttack();
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

void UPlayerCombatComponent::SetNextAttack()
{
	if(AvailableAttackTypes.IsEmpty())
	{
		return;
	}
	
	int32 CurrentIndex = GetIndexOfAttack(GetCurrentAttackType());
	if(AvailableAttackTypes.IsValidIndex(CurrentIndex))
	{
		CurrentIndex = (CurrentIndex + 1) % AvailableAttackTypes.Num();
		SetCurrentAttackByIndex(CurrentIndex);
	}
}

void UPlayerCombatComponent::SetPreviousAttack()
{
	if(AvailableAttackTypes.IsEmpty())
	{
		return;
	}

	int32 CurrentIndex = GetIndexOfAttack(GetCurrentAttackType());
	if(AvailableAttackTypes.IsValidIndex(CurrentIndex))
	{
		CurrentIndex = (CurrentIndex - 1 + AvailableAttackTypes.Num()) % AvailableAttackTypes.Num();
		SetCurrentAttackByIndex(CurrentIndex);
	}
}

void UPlayerCombatComponent::GrantPlayerAttackTypeByClass(TSubclassOf<UFPAttackType> InAttackTypeClass, bool bSetAsCurrentAttack)
{
	if(UFPAttackType* GrantedAttack = GrantAttackTypeByClass(InAttackTypeClass))
	{
		if(bSetAsCurrentAttack)
		{
			SetCurrentAttack(GrantedAttack);
		}
	}
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

void UPlayerCombatComponent::SetCurrentAttackByIndex(int32 InAttackIndex)
{
	if(AvailableAttackTypes.IsValidIndex(InAttackIndex))
	{
		SetCurrentAttack(AvailableAttackTypes[InAttackIndex]);
	}
}

void UPlayerCombatComponent::SetCurrentAttack(UFPAttackType* InAttackType)
{
	if(InAttackType && CurrentAttackTag.IsValid() && InAttackType->GetAttackTypeTag() == CurrentAttackTag)
	{
		return;
	}
	
	if(UFPAttackType* CurrentAttack = GetCurrentAttackType())
	{
		if(CurrentAttack->IsActive())
		{
			CurrentAttack->EndAttack();
		}
	}

	CurrentAttackTag = InAttackType ? InAttackType->GetAttackTypeTag() : FGameplayTag::EmptyTag;
}

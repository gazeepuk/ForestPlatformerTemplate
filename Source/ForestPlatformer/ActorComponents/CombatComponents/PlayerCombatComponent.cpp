// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AttackTypes/FPAttackType.h"
#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"

bool UPlayerCombatComponent::CanAttack_Implementation() const
{
	return Super::CanAttack_Implementation() && CurrentPrimaryAttack && CurrentPrimaryAttack->CanAttack();
}

bool UPlayerCombatComponent::TryPerformCurrentAttack()
{
	if(CanAttack())
	{
		UFPFunctionLibrary::NativeAddGameplayTagToActor(GetOwner(), FPGameplayTags::Shared_Status_Attacking);
		
		CurrentPrimaryAttack.Get()->PerformAttack(nullptr);

		return true;
	}
	
	return false;
}

void UPlayerCombatComponent::SetPrimaryAttackByClass(TSubclassOf<UFPAttackType> InPrimaryAttackClass)
{
	if(CurrentPrimaryAttack.GetClass() == InPrimaryAttackClass)
	{
		return;
	}
	
	// Cleanup previous Attack
	if(CurrentPrimaryAttack)
	{
		CurrentPrimaryAttack->OnAttackEnded.RemoveDynamic(this, &ThisClass::OnAttackEnded);
		CurrentPrimaryAttack->ConditionalBeginDestroy();
		CurrentPrimaryAttack = nullptr;
	}

	if(InPrimaryAttackClass)
	{
		// Create and Init New Attack
		CurrentPrimaryAttack = NewObject<UFPAttackType>(this, InPrimaryAttackClass);
		if(CurrentPrimaryAttack)
		{
			CurrentPrimaryAttack->InitAttack(GetOwner(), this);
			CurrentPrimaryAttack->OnAttackEnded.AddUniqueDynamic(this, &ThisClass::OnAttackEnded);
		}
	}
}

void UPlayerCombatComponent::ResetPrimaryAttack()
{
	if(DefaultAttackType)
	{
		SetPrimaryAttackByClass(DefaultAttackType);
	}
}

void UPlayerCombatComponent::InitCombatComponent()
{
	BindCombatInput();
	SetPrimaryAttackByClass(DefaultAttackType);
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

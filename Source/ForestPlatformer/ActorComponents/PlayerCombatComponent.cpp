// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPAttackType.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
}

bool UPlayerCombatComponent::CanAttack() const
{
	return Super::CanAttack() && CurrentPrimaryAttack && CurrentPrimaryAttack->CanAttack(GetOwner());
}

bool UPlayerCombatComponent::TryCurrentAttack()
{
	if(CanAttack())
	{
		bAttacking = true;
		
		CurrentPrimaryAttack.Get()->PerformAttack(nullptr);

		return true;
	}
	
	return false;
}

void UPlayerCombatComponent::SetPrimaryAttackByClass(TSubclassOf<UFPAttackType> InPrimaryAttackClass)
{
	if(InPrimaryAttackClass)
	{
		CurrentPrimaryAttack = NewObject<UFPAttackType>(GetOwner(), InPrimaryAttackClass);
		if(CurrentPrimaryAttack)
		{
			CurrentPrimaryAttack->SetOwningActor(GetOwner());
			CurrentPrimaryAttack->OnAttackEnded.AddUniqueDynamic(this, &ThisClass::OnAttackEnded);
		}
	}
}

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	BindCombatAction();

	SetPrimaryAttackByClass(DefaultAttackType);
}

void UPlayerCombatComponent::AttackAction_Started(const FInputActionValue& ActionValue)
{
	TryCurrentAttack();
}

void UPlayerCombatComponent::BindCombatAction()
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
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	if(InputSubsystem && EnhancedInputComponent)
	{
		InputSubsystem->AddMappingContext(CombatMappingContext, CombatInputPriority);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AttackAction_Started);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/CombatComponents/PlayerCombatComponent.h"
#include "ActorComponents/FPCharacterMovementComponent.h"
#include "ActorComponents/FPSpringArmComponent.h"
#include "ActorComponents/EffectComponent/FPEffectComponent.h"
#include "ActorComponents/HealthComponent/HealthComponent.h"
#include "ActorComponents/InteractableComponents/PlayerInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "CoreTypes/FPCustomCollisions.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/PlayerControllers/FPPlayerController.h"

AFPPlayerCharacter::AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UFPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CameraBoom = CreateDefaultSubobject<UFPSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
	CameraComponent->bUsePawnControlRotation = false;

	GetMesh()->SetCollisionObjectType(ECC_FP_Player_OC);

	InteractionCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollisionComponent"));
	InteractionCollisionComponent->SetupAttachment(GetRootComponent());
	InteractionCollisionComponent->SetGenerateOverlapEvents(true);
	InteractionCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionCollisionComponent->SetCollisionResponseToChannel(ECC_FP_Interactable_OC, ECR_Overlap);
	
	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("PlayerInteractionComponent"));

	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComponent"));

	EffectComponent = CreateDefaultSubobject<UFPEffectComponent>(TEXT("EffectComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnZeroHealth.AddUniqueDynamic(this, &ThisClass::OnDeath);
	HealthComponent->OnTakeDamage.AddUniqueDynamic(this, &ThisClass::OnTakeDamage);
}

#pragma region ICoinsWalletInterface
void AFPPlayerCharacter::AddCoins_Implementation(int32 InCoinsNum)
{
	ICoinsWalletInterface::Execute_AddCoins(GetController(), InCoinsNum);
}

bool AFPPlayerCharacter::TrySpendCoins_Implementation(int32 InCoinsNum)
{
	return ICoinsWalletInterface::Execute_TrySpendCoins(GetController(), InCoinsNum);
}

bool AFPPlayerCharacter::HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const
{
	return ICoinsWalletInterface::Execute_HasEnoughCoins(GetController(), InCoinsNumToSpend);
}

void AFPPlayerCharacter::SetCurrentCoins_Implementation(int32 InNewCurrentCoins)
{
	ICoinsWalletInterface::Execute_SetCurrentCoins(GetController(), InNewCurrentCoins);
}

int32 AFPPlayerCharacter::GetCurrentCoins_Implementation() const
{
	return ICoinsWalletInterface::Execute_GetCurrentCoins(GetController());
}

#pragma endregion

void AFPPlayerCharacter::TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatedBy)
{
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageCauser, InDamage, InstigatedBy);
	}
}

bool AFPPlayerCharacter::CanInteract_Implementation() const
{
	return HealthComponent && HealthComponent->IsAlive();
}

void AFPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(PlayerInteractionComponent)
	{
		PlayerInteractionComponent->SetInteractionCollision(InteractionCollisionComponent);
	}
}

void AFPPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AFPPlayerController* FPPlayerController = Cast<AFPPlayerController>(NewController))
	{
		FPPlayerController->InitHUDWidget();
		FPPlayerController->InitHealthBar(HealthComponent);
		CombatComponent->InitCombatComponent();
		PlayerInteractionComponent->BindInteractionAction();
	}
}

void AFPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);

	if(MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPPlayerCharacter::MoveAction_Triggered);
	}
	if(LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPPlayerCharacter::LookAction_Triggered);
	}
	if(JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPPlayerCharacter::JumpAction_Started);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPPlayerCharacter::JumpAction_Completed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFPPlayerCharacter::JumpAction_Triggered);
	}
	if(ZoomCameraMouseAction)
	{
		EnhancedInputComponent->BindAction(ZoomCameraMouseAction, ETriggerEvent::Started, this, &AFPPlayerCharacter::ZoomCameraMouseAction_Started);
	}
	if(ZoomCameraControllerAction)
	{
		EnhancedInputComponent->BindAction(ZoomCameraControllerAction, ETriggerEvent::Triggered, this, &AFPPlayerCharacter::ZoomCameraControllerAction_Triggered);
		EnhancedInputComponent->BindAction(ZoomCameraControllerAction, ETriggerEvent::Completed, this, &AFPPlayerCharacter::ZoomCameraControllerAction_Completed);
	}
}

void AFPPlayerCharacter::OnDeath_Implementation()
{
	if(CombatComponent)
	{
		CombatComponent->TryAbortActiveAttack();
	}
}

void AFPPlayerCharacter::OnTakeDamage_Implementation(float DamageValue)
{
	if(CombatComponent)
	{
		CombatComponent->TryAbortActiveAttack();
	}
}

void AFPPlayerCharacter::MoveAction_Triggered(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFPPlayerCharacter::LookAction_Triggered(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPPlayerCharacter::JumpAction_Started(const FInputActionValue& InputActionValue)
{
	Jump();
}

void AFPPlayerCharacter::JumpAction_Triggered(const FInputActionValue& InputActionValue)
{
	GetCharacterMovement<UFPCharacterMovementComponent>()->HandleFloating();
}

void AFPPlayerCharacter::JumpAction_Completed(const FInputActionValue& InputActionValue)
{
	StopJumping();
	
	GetCharacterMovement<UFPCharacterMovementComponent>()->StopFloating();
}

void AFPPlayerCharacter::ZoomCameraMouseAction_Started(const FInputActionValue& InputActionValue)
{
	CameraBoom->ZoomMouse(InputActionValue.Get<float>());
}

void AFPPlayerCharacter::ZoomCameraControllerAction_Triggered(const FInputActionValue& InputActionValue)
{
	CameraBoom->ZoomController(InputActionValue.Get<float>());
}

void AFPPlayerCharacter::ZoomCameraControllerAction_Completed(const FInputActionValue& InputActionValue)
{
	CameraBoom->StopZooming();
}

void AFPPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	GetCharacterMovement<UFPCharacterMovementComponent>()->StopFloating();
}


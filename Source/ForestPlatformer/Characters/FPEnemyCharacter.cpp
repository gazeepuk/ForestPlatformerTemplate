// Developed by Ivan Piankouski. All Rights Reserved


#include "FPEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActorComponents/CharacterMovementComponents/FPCharacterMovementComponent.h"
#include "ActorComponents/CombatComponents/AICombatComponent.h"
#include "ActorComponents/EffectComponent/FPEffectComponent.h"
#include "ActorComponents/HealthComponent/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreTypes/CustomCollisions/FPCustomCollisions.h"
#include "CoreTypes/GameplayTags/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Subsystems/SaveGameSubsystem.h"


AFPEnemyCharacter::AFPEnemyCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UFPCharacterMovementComponent>(CharacterMovementComponentName))
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_FP_Damageable_TC, ECR_Block);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EffectComponent = CreateDefaultSubobject<UFPEffectComponent>(TEXT("EffectComponent"));
	AICombatComponent = CreateDefaultSubobject<UAICombatComponent>(TEXT("AICombatComponent"));
	
	bDefeated = false;
}

void AFPEnemyCharacter::TakeDamage_Implementation(AActor* DamageCauser, float InDamage, AController* InstigatedBy)
{
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageCauser, InDamage, InstigatedBy);
	}
}

FFPSavableData AFPEnemyCharacter::GetSaveData_Implementation() const
{
	FFPSavableData SavableData;
	SavableData.bActive = !bDefeated;
	SavableData.ObjectType = "Enemy";
	
	return SavableData;
}

void AFPEnemyCharacter::LoadFromSaveData_Implementation(const FFPSavableData& SaveData)
{
	bDefeated = !SaveData.bActive;

	Execute_OnLoadedFromSaveData(this);
}

void AFPEnemyCharacter::OnLoadedFromSaveData_Implementation()
{
	if(bDefeated)
	{
		if(bDestroyAfterDefeating)
		{
			Destroy();
		}
		else
		{
			DisableEnemy();
		}
	}
}

void AFPEnemyCharacter::DisableEnemy()
{
	UFPFunctionLibrary::NativeAddGameplayTagToActor(this, FPGameplayTags::Shared_Status_Dead);

	if(AAIController* AIController = GetController<AAIController>())
	{
		AIController->StopMovement();
		if(UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic("Disabled");
		}
		if(UAIPerceptionComponent* AIPerception = AIController->GetAIPerceptionComponent())
		{
			AIPerception->UnregisterComponent();
		}
	}
	
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void AFPEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnZeroHealth.AddUniqueDynamic(this, &ThisClass::OnDeath);
	HealthComponent->OnTakeDamage.AddUniqueDynamic(this, &ThisClass::OnTakeDamage);
}

void AFPEnemyCharacter::Destroyed()
{
	if(bSaveAfterDefeating)
	{
		if(UGameInstance* GameInstance = GetGameInstance())
		{
			if(USaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<USaveGameSubsystem>())
			{
				SaveGameSubsystem->AddPendingSavableActor(this);
			}
		}
	}
	
	Super::Destroyed();
}

void AFPEnemyCharacter::OnDeath_Implementation()
{
	bDefeated = true;
	SetActorEnableCollision(false);
	if(AICombatComponent)
	{
		AICombatComponent->TryAbortActiveAttack();
	}
}

void AFPEnemyCharacter::OnTakeDamage_Implementation(float DamageValue)
{
	if(AICombatComponent)
	{
		AICombatComponent->TryAbortActiveAttack();
	}
}

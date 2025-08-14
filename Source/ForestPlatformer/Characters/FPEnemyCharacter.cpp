// Fill out your copyright notice in the Description page of Project Settings.


#include "FPEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActorComponents/EffectComponent/FPEffectComponent.h"
#include "ActorComponents/HealthComponent/HealthComponent.h"
#include "CoreTypes/FPGameplayTags.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "GameModes/FPGameMode.h"
#include "Perception/AIPerceptionComponent.h"
#include "Subsystems/SaveGameSubsystem.h"


AFPEnemyCharacter::AFPEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EffectComponent = CreateDefaultSubobject<UFPEffectComponent>(TEXT("EffectComponent"));
	
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

void AFPEnemyCharacter::Destroyed()
{
	if(bSaveAfterDefeating)
	{
		if(USaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
		{
			SaveGameSubsystem->AddPendingSavableObjects(this);
		}
	}
	
	Super::Destroyed();
}

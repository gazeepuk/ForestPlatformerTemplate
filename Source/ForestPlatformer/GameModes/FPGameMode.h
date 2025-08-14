// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/FPSaveGame.h"
#include "FPGameMode.generated.h"

class ISavableObjectInterface;
class AFPCheckpoint;
class UFPSaveGame;
/**
 * 
 */
UCLASS()
class FORESTPLATFORMER_API AFPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable)
	void RegisterCheckpoint(AFPCheckpoint* InCheckpoint);

	UFUNCTION(BlueprintCallable)
	void InitGameFromSave();

	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetLastCheckpointID() const { return LastCheckpointID; }
protected:
	virtual void BeginPlay() override;
	
	template<std::derived_from<UFPSaveGame> T>
	T* GetOrCreateSaveData()
	{
		T* SaveData = nullptr;

		if(UGameplayStatics::DoesSaveGameExist("SaveSlot", 0))
		{
			SaveData = Cast<T>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));

			if(!SaveData)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to load existing save. Creating a new save"));
			}
		}

		if(!SaveData)
		{
			SaveData = Cast<T>(UGameplayStatics::CreateSaveGameObject(T::StaticClass()));
			if(!SaveData)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create a new save data"));
				return nullptr;
			}
		}
		
		return SaveData;
	}

	UPROPERTY()
	TArray<AFPCheckpoint*> AllCheckpoints;
	FName LastCheckpointID;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> LastActiveCheckpoint;
	FTransform LastCheckpointTransform;

	TMap<FName, FFPSavableData> PendingSavableData;
};

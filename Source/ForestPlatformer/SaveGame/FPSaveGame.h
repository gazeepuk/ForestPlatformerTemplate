// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "GameFramework/SaveGame.h"
#include "FPSaveGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FFPSavableData
{
	GENERATED_BODY()

	FFPSavableData() : ObjectType(NAME_None), bActive(true) {}
	
	UPROPERTY()
	FName ObjectType;

	UPROPERTY()
	bool bActive; 

	// JSON string
	UPROPERTY()
	FString CustomData;

	FString ToJsonString() const
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		const bool bConvertedSuccessfully = FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, JsonObject.ToSharedRef());

		if(!bConvertedSuccessfully)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to convert FFPSavableData to JSON"));
			return FString();
		}
		
		FString OutputString;
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

		return OutputString;
	}

	static FFPSavableData FromJsonString(const FString& JsonString)
	{
		FFPSavableData SavableData;

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

		if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			if(!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StaticStruct(), &SavableData))
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to convert JSON to FFPSavableData"));
			}
		}

		return SavableData;
	}
};

USTRUCT(BlueprintType)
struct FFPLevelData
{
	GENERATED_BODY()

	FFPLevelData() : LastActiveCheckpointID(NAME_None), bLevelCompleted(false){}

	bool TrySaveActor(const AActor* InActor);
	
	UPROPERTY()
	FName LastActiveCheckpointID;

	UPROPERTY()
	TMap<FName, FFPSavableData> SavedObjects;

	UPROPERTY()
	bool bLevelCompleted;
};

UCLASS()
class FORESTPLATFORMER_API UFPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TMap<FString, FFPLevelData> LevelDataMap;

	UPROPERTY()
	FString CurrentLevelName;
};

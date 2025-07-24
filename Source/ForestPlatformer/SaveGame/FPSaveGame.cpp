// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/FPSaveGame.h"

#include "Interfaces/SavableObjectInterface.h"

bool FFPLevelData::TrySaveActor(const AActor* InActor)
{
	if(InActor && InActor->Implements<USavableObjectInterface>())
	{
		FName SaveID = ISavableObjectInterface::Execute_GetSaveID(InActor);
		FFPSavableData NewData = ISavableObjectInterface::Execute_GetSaveData(InActor);
		bool bShouldSave = ISavableObjectInterface::Execute_ShouldSave(InActor);
		
		if(SaveID != NAME_None && bShouldSave)
		{
			FFPSavableData& ExistingData = SavedObjects.FindOrAdd(SaveID);
			ExistingData = NewData;

			return true;
		}
	}

	return false;
}

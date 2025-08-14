// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/FPSaveGame.h"

#include "Interfaces/SavableObjectInterface.h"

bool FFPLevelData::TrySaveActor(const AActor* InActor)
{
	if(!InActor || !InActor->Implements<USavableObjectInterface>())
	{
		return false;
	}
	
	FName SaveID = ISavableObjectInterface::Execute_GetSaveID(InActor);
	if(SaveID.IsNone() || !ISavableObjectInterface::Execute_ShouldSave(InActor))
	{
		return false;
	}
	
	FFPSavableData NewData = ISavableObjectInterface::Execute_GetSaveData(InActor);
	SavedObjects.FindOrAdd(SaveID) = NewData;
	return true;
}

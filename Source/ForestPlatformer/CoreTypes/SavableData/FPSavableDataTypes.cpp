#include "FPSavableDataTypes.h"

#include "Interfaces/SavableActorInterface.h"

bool FFPLevelData::TrySaveActor(const AActor* InActor)
{
	if(!InActor || !InActor->Implements<USavableActorInterface>())
	{
		return false;
	}

	const FName SaveID = ISavableActorInterface::Execute_GetSaveID(InActor);
	if(SaveID.IsNone() || !ISavableActorInterface::Execute_ShouldSave(InActor))
	{
		return false;
	}
	
	FFPSavableData NewData = ISavableActorInterface::Execute_GetSaveData(InActor);
	SavedObjects.FindOrAdd(SaveID) = NewData;
	return true;
}

// Developed by Ivan Piankouski. All Rights Reserved


#include "SavableActorInterface.h"

#include "FunctionLibrary/FPFunctionLibrary.h"


// Add default functionality here for any ISavableObjectInterface functions that are not pure virtual.
void ISavableActorInterface::InitializeSaveID()
{
	if(GetSaveID_Implementation() != NAME_None)
	{
		return;
	}

	if(const AActor* Actor = Cast<AActor>(this))
	{
		const FName AutoSaveID = UFPFunctionLibrary::GenerateSaveID(Actor);

		SetSaveID_Implementation(AutoSaveID);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SavableObjectInterface.h"


// Add default functionality here for any ISavableObjectInterface functions that are not pure virtual.
void ISavableObjectInterface::InitializeSaveID()
{
	if(GetSaveID_Implementation() != NAME_None)
	{
		return;
	}

	if(AActor* Actor = Cast<AActor>(this))
	{
		const FVector Location = Actor->GetActorLocation();
		const FName AutoSaveID = FName(FString::Printf(
			TEXT("%s_%d_%d_%d"),
			*Actor->GetClass()->GetName(),
			FMath::RoundToInt32(Location.X),
			FMath::RoundToInt32(Location.Y),
			FMath::RoundToInt32(Location.Z)));

		SetSaveID_Implementation(AutoSaveID);
	}
}

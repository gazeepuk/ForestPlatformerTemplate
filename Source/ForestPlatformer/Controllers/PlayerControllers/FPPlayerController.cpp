// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerController.h"

AFPPlayerController::AFPPlayerController()
{
	PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId AFPPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

ETeamAttitude::Type AFPPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if(!OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}
	
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());
	if(OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AFPPlayerController::AddCoins_Implementation(int32 InCoinsNum)
{
	SetCurrentCoins_Implementation(InCoinsNum + CurrentCoins);
}

bool AFPPlayerController::TrySpendCoins_Implementation(int32 InCoinsNum)
{
	if(HasEnoughCoins_Implementation(InCoinsNum))
	{
		SetCurrentCoins_Implementation(CurrentCoins - InCoinsNum);
		return true;
	}
	return false;
}

bool AFPPlayerController::HasEnoughCoins_Implementation(int32 InCoinsNumToSpend) const
{
	return InCoinsNumToSpend <= CurrentCoins;
}

void AFPPlayerController::SetCurrentCoins_Implementation(int32 InNewCurrentCoins)
{
	CurrentCoins = FMath::Clamp(InNewCurrentCoins, 0, MaxCoins);
	OnCurrentCoinsChanged(CurrentCoins);
	OnCurrentCoinsChangedDelegate.Broadcast(CurrentCoins);;
}

int32 AFPPlayerController::GetCurrentCoins_Implementation() const
{
	return CurrentCoins;
}


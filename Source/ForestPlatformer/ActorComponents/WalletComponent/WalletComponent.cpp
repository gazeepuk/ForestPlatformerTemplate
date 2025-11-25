// Fill out your copyright notice in the Description page of Project Settings.


#include "WalletComponent.h"


void UWalletComponent::AddCoins(int32 InCoins)
{
	SetCurrentCoins(CurrentCoins + InCoins);
}

void UWalletComponent::SpendCoins(int32 InCoins)
{
	SetCurrentCoins(CurrentCoins - InCoins);
}

void UWalletComponent::SetCurrentCoins(int32 InCoins)
{

	int32 MinValue = bCanBeNegative ? INT32_MIN : 0;
	int32 MaxValue = bClampMaxValue ? MaxCoins : INT32_MAX;

	if(MaxValue < MinValue)
	{
		MaxValue = MinValue;
		UE_LOG(LogTemp, Warning, TEXT("Coins maximum clamping value {%d} is less than minimal clamping value {%d}. The maximum value is set to the minimal value"), MaxValue, MinValue);
	}
	
	InCoins = FMath::Clamp(InCoins, MinValue, MaxValue);
	
	if(CurrentCoins != InCoins)
	{
		int32 OldCoins = CurrentCoins;
		CurrentCoins = InCoins;
		
		OnCurrentCoinsChanged.Broadcast(CurrentCoins, MaxCoins, CurrentCoins - OldCoins);
	}
}

void UWalletComponent::SetMaxCoins(int32 InCoins)
{
	MaxCoins = InCoins;
	
	// Set the same current coins amount to clamp it
	SetCurrentCoins(CurrentCoins);
}

bool UWalletComponent::HasEnoughCoins(const int32 InCoinsNumInSpend) const
{
	return CurrentCoins >= InCoinsNumInSpend;
}

int32 UWalletComponent::GetCurrentCoins() const
{
	return CurrentCoins;
}

int32 UWalletComponent::GetMaxCoins() const
{
	return MaxCoins;
}

void UWalletComponent::SetCanBeNegative(bool bNewCanBeNegative)
{
	if(bNewCanBeNegative != bCanBeNegative)
	{
		bCanBeNegative = bNewCanBeNegative;
		if(!bCanBeNegative)
		{
			// Set the same current coins amount to clamp it
			SetCurrentCoins(CurrentCoins);
		}
	}
}

void UWalletComponent::SetClampingMaxValue(bool bNewClampCurrentCoins)
{
	if(bNewClampCurrentCoins != bClampMaxValue)
	{
		bClampMaxValue = bNewClampCurrentCoins;
		if(bClampMaxValue)
		{
			// Set the same current coins amount to clamp it
			SetCurrentCoins(CurrentCoins);
		}
	}
}

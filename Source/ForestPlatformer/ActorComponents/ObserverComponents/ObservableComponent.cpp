// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ObserverComponents/ObservableComponent.h"

UObservableComponent::UObservableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObservableComponent::SetObservableActive(bool bNewActive)
{
	if(bObservableActive == bNewActive)
	{
		return;
	}

	bObservableActive = bNewActive;
	OnObservableStateChanged.Broadcast(this, bObservableActive);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ObserverComponents/ObservableComponent.h"

UObservableComponent::UObservableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObservableComponent::TriggerObservable()
{
	if(!bTriggered)
	{
		OnObservableTriggered.Broadcast(this);

		bTriggered = true;
	}
}

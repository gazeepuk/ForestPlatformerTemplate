// Developed by Ivan Piankouski. All Rights Reserved


#include "ActorComponents/ObserverComponents/ObservableComponent.h"

#include "CoreTypes/ObservingTypes/ObservableStates/ObservableState.h"

UObservableComponent::UObservableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObservableComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ensures that the component has a valid state
	if(ensureMsgf(ObservableState,
		TEXT("Observer State for %s actor is nullptr. Assign the state object, otherwise checking results with any observable condition will be false"),
		*GetNameSafe(GetOwner())))
	{
		ObservableState->OnStateChanged.AddUniqueDynamic(this, &ThisClass::ObservableStateChanged);
	}
}

void UObservableComponent::ObservableStateChanged()
{
	OnObservableStateChanged.Broadcast(this);
}

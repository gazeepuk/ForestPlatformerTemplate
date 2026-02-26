// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableComponent.generated.h"

class UObservableState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObservableStateChanged, UObservableComponent*, ObservableActor);

/**
 * Component that represents an observable entity which can be triggered
 * and notifies observers when its state changes to triggered
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTPLATFORMER_API UObservableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UObservableComponent();
	
	/** Delegate that broadcasts when this component is triggered */
	UPROPERTY(BlueprintAssignable, Category = "Observable")
	FOnObservableStateChanged OnObservableStateChanged;

	/** Returns the observable state */
	UFUNCTION(BlueprintPure, Category = "Observable")
	FORCEINLINE UObservableState* GetObservableState() const { return ObservableState; }
	
protected:
	virtual void BeginPlay() override;

	/** State that has own value to compare */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Observable")
	TObjectPtr<UObservableState> ObservableState;

private:
	/** Callback function for the state changes */
	UFUNCTION()
	void ObservableStateChanged();
};

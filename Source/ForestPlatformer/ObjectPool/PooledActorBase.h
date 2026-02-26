// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActorBase.generated.h"

UCLASS()
class FORESTPLATFORMER_API APooledActorBase : public AActor
{
	GENERATED_BODY()

public:
	/* Default constructor for APooledActorBase */
	APooledActorBase();

	/* Sets the actor active or not */
	UFUNCTION(BlueprintCallable, Category = "PooledActor")
	void SetPooledActorActive(bool InActive);

	/* Returns bActive */
	UFUNCTION(BlueprintPure, Category = "PooledActor")
	FORCEINLINE bool IsActive() const { return bActive; }
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorActivated();

	UFUNCTION(BlueprintNativeEvent)
	void OnActorDeactivated();

private:
	/* Lifetime after activation */
	UPROPERTY(EditDefaultsOnly, Category = "PooledActor")
	float ActiveLifeTime = 10.f;
	
	bool bActive = true;

	FTimerHandle DeactivateActorTimerHandle;

	void ActivateActor();
	void DeactivateActor();
};

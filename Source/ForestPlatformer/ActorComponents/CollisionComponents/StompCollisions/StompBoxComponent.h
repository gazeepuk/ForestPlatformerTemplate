// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "StompBoxComponent.generated.h"

class UArrowComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FStompBoxOverlappedDelegate, UPrimitiveComponent*, OverlappedComp, AActor*,
                                             OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool,
                                             bFromSweep, const FHitResult&, SweepResult);

/**
 * Specialized collision component that detects when actors overlap from a specific direction.
 * Used for implementing stomp attacks, bounce pads, damageable spots
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UStompBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UStompBoxComponent();

	/** Delegate that broadcasts when an actor overlaps this component from above */
	UPROPERTY(BlueprintAssignable)
	FStompBoxOverlappedDelegate OnStompBoxOverlappedActorAbove;
	
protected:

	/** Determines if the specified	actor is positioned above this component */
	virtual bool IsActorAbove(const AActor* InActor);

	/**	Vertical tolerance buffer for detecting "above" positioning */
	UPROPERTY(EditDefaultsOnly)
	float StompHeightBuffer = 5.f;
private:
	/** Callback function triggered when any actor begins overlapping this component */
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

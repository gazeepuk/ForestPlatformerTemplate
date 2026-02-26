// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"

class UPatrolDataBase;

/**
 * Determines the patrol type.
 */
UENUM()
enum class EPatrolType : uint8
{
	None,		// Patrolling is disabled
	Sequence,	// Sequential patrol
	InBounds	// Random area patrol
};

/**
 * Component that manages patrol behavior for AI characters,
 * providing waypoint navigation and random movement
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Finds next patrol location for the current patrol type
	 * @note Override FindNextPatrolLocation_Implementation for your custom logic
	 * @return True, if the found location is valid, false otherwise */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool FindNextPatrolLocation(FVector& OutFoundLocation);

	/** Checks if the current patrol index points to a valid patrol point */
	UFUNCTION(BlueprintPure)
	bool IsCurrentPatrolIndexValid() const;

	/** Advances to the next patrol point in the sequence. Clamps the index or sets -1, if the patrol points array is empty */
	UFUNCTION(BlueprintCallable)
	void SetNextPatrolPointIndex();
	
	/** Returns the world of the current patrol point
	* @return Current patrol point location if it's valid, zero vector otherwise
	*/
	UFUNCTION(BlueprintPure)
	FVector GetCurrentPatrolPointLocation() const;

	/** Returns the world location of a patrol point by its index
	* @return Patrol point location if it's valid, zero vector otherwise
	*/
	UFUNCTION(BlueprintPure)
	FVector GetPatrolPointLocationByIndex(int32 InPatrolPointIndex) const;

	/** Determines the current patrol type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	EPatrolType PatrolType = EPatrolType::InBounds;

	/**	Direction of patrol progression (true = forward, false = backward ) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol|Sequence")
	bool bPatrolForward = true;

protected:
	/** Sets the last patrol point of the array as the current point */
	virtual void BeginPlay() override;

	/** Volume that defines the boundaries for random patrol movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol|Bounds")
	TObjectPtr<AVolume> PatrolBorders;

	/** Array of actor waypoints that define the patrol route */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol|Sequence")
	TArray<AActor*> PatrolPoints;

	/** Index of the current patrol point in the PatrolPoints array */
	UPROPERTY(BlueprintReadOnly, Category = "Patrol|Sequence")
	int32 CurrentPatrolPointIndex = -1;

	/** Finds next patrol location for EPatrolType::Sequence */
	bool FindNextPatrolLocationInSequence(FVector& OutFoundLocation);
	
	/** Finds next patrol location for EPatrolType::InBounds */
	bool FindNextPatrolLocationInBounds(FVector& OutFoundLocation) const;
};

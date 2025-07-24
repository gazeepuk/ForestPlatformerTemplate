// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool/PooledActorBase.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class FORESTPLATFORMER_API AProjectileBase : public APooledActorBase
{
	GENERATED_BODY()

public:
	/** Default constructor for APooledActorBase*/
	AProjectileBase();

	/**
	 * Launch a projectile with input movement parameters.
	 *
	 * @param InLocation		World space spawn location of the projectile
	 * @param InDirection		Direction vector
	 * @param InInitialSpeed	Initial Speed (use -1 for DefaultInitialSpeed)
	 * @param InMaxSpeed		Max Speed (use -1 for DefaultMaxSpeed)
	 * @param InGravityScale	GravityScale (use -1 for DefaultGravityScale)
	 */
	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector InLocation, FVector InDirection, float InInitialSpeed = -1.f, float InMaxSpeed = -1.f, float InGravityScale = -1.f);
	
protected:
	/**
	 * Set projectile's collision to ignore owner if bIgnoreOwner is true.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * Activates ProjectileMovementComponent and plays a flight sound.
	 * @note Must call Super::ActivateActor_Implementation for proper pooling logic
	 */
	virtual void ActivateActor_Implementation() override;
	/**
	 * Deactivates ProjectileMovementComponent, resets its movement properties, and stops the flight sound.
	 * @note Must call Super::DeactivateActor_Implementation for proper pooling logic. Use OnDeactivateProjectile instead for deactivating the projectile.
	 */
	virtual void DeactivateActor_Implementation() override;

	/**
	 * Called when the projectile overlaps other actor.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/**
	 * Called when the projectile hits other actor.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/**
	 * Contains the main projectile interaction logic
	 * @return Return true if the interaction was successful
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool ProjectileInteract(AActor* InInteractingActor);

	/**
	 * Calls DeactivateActor. Use for adding deactivation VFX, SFX.
	 * @note Ensure parent function is called.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDeactivateProjectile();

	/**
	 * Calls after the projectile's impact. Use for adding impact VFX, SFX and other.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileImpact();

	/** Sets ProjectileFlightSound as FlightAudioComponent's sound or fade in, if it's faded out. */
	void PlayFlightSound();
	/** Plays ProjectileFlightSound at projectile's location. */
	void PlayImpactSound();

	/** Projectile's collision. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Collision")
	TObjectPtr<USphereComponent> ProjectileCollision;
	/** Projectile's movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	/** If it's true, the projectile damage only hostile actors. Uses IGenericTeamAgentInterface for checking the attitudes. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
	bool bDamageOnlyHostilePawns = true;

	/** Projectile's damage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
	float DamageValue = 1.f;

	/** If it's true, ignores all collision events with owner. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Collision")
	bool bIgnoreOwner = true;

	/** If It's true, the projectile continues moving after overlapping an actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Collision")
	bool bPierceActors = false;

	/** Projectile's impact sound. Plays with PlayImpactSound function. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	TObjectPtr<USoundBase> ProjectileImpactSound;

	/** Projectile's flight sound. Plays with PlayFlightSound function. Used for FlightAudioComponent. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	TObjectPtr<USoundBase> ProjectileFlightSound;

	/** Attached to the projectile for playing ProjectileFlightSound. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	TObjectPtr<UAudioComponent> FlightAudioComponent;
private:

	/** Projectile's default max speed. Used for setting ProjectileMovementComponent's MaxSpeed*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Default")
	float DefaultMaxSpeed;
	/** Projectile's default initial speed. Used for setting ProjectileMovementComponent's InitialSpeed*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Default")
	float DefaultInitialSpeed;
	/** Projectile's default gravity scale. Used for setting ProjectileMovementComponent's GravityScale*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Default")
	float DefaultGravityScale;
};

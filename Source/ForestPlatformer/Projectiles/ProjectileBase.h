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
	AProjectileBase();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector InLocation, FVector InDirection, float InInitialSpeed = -1.f, float InMaxSpeed = -1.f, float InGravityScale = -1.f);
	
protected:
	virtual void BeginPlay() override;
	
	virtual void ActivateActor_Implementation() override;
	virtual void DeactivateActor_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintNativeEvent)
	bool ProjectileInteract(AActor* InInteractingActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnDeactivateProjectile();

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileImpact();
	
	void PlayFlightSound();
	void PlayImpactSound();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Collision")
	TObjectPtr<USphereComponent> ProjectileCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
	bool bDamageOnlyHostileActors = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
	float DamageValue = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Collision")
	bool bIgnoreOwner = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Collision")
	bool bPierceActors = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	TObjectPtr<USoundBase> ProjectileImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	TObjectPtr<USoundBase> ProjectileFlightSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	TObjectPtr<UAudioComponent> FlightAudioComponent;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Default")
	float DefaultMaxSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Default")
	float DefaultInitialSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Default")
	float DefaultGravityScale;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "CoreTypes/FPCustomCollisions.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


AProjectileBase::AProjectileBase()
{
	// Create Projectile Collision
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionObjectType(ECC_FP_Projectile_OC);
	ProjectileCollision->SetCollisionResponseToChannel(ECC_FP_Projectile_OC, ECR_Ignore);
	ProjectileCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
	ProjectileCollision->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	SetRootComponent(ProjectileCollision);

	// Create ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetActive(false);

	// Create FlightAudioComponent 
	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FlightAudioComponent"));
	FlightAudioComponent->SetupAttachment(GetRootComponent());
	FlightAudioComponent->bAutoActivate = false;
	FlightAudioComponent->bAllowSpatialization = true;
}

void AProjectileBase::LaunchProjectile(FVector InLocation, FVector InDirection, float InInitialSpeed, float InMaxSpeed,
	float InGravityScale)
{

	SetActorLocationAndRotation(InLocation, InDirection.Rotation());

	// Set projectile movement properties
	ProjectileMovementComponent->InitialSpeed = InInitialSpeed > 0.f ? InInitialSpeed : DefaultInitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InMaxSpeed > 0.f ? InMaxSpeed : DefaultMaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = InGravityScale > 0.f ? InGravityScale: DefaultGravityScale;
	
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovementComponent->InitialSpeed);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		ProjectileCollision->IgnoreActorWhenMoving(GetOwner(), bIgnoreOwner);
	}
}

void AProjectileBase::OnActorActivated_Implementation()
{
	Super::OnActorActivated_Implementation();

	OnProjectileActivated();
	
	ProjectileMovementComponent->SetActive(true);
}

void AProjectileBase::OnActorDeactivated_Implementation()
{
	OnProjectileDeactivated();

	// Disable movement
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->StopMovementImmediately();
	
	ProjectileMovementComponent->SetActive(false);

	Super::OnActorDeactivated_Implementation();
}

void AProjectileBase::OnProjectileActivated_Implementation()
{
	PlayFlightSound();
}

void AProjectileBase::OnProjectileDeactivated_Implementation()
{
	FlightAudioComponent->Stop();
	FlightAudioComponent->SetSound(nullptr);
}

void AProjectileBase::PlayFlightSound() const
{
	if(FlightAudioComponent)
	{
		if(FlightAudioComponent->Sound == nullptr)
		{
			FlightAudioComponent->SetSound(ProjectileFlightSound);
		}

		if(FlightAudioComponent->IsPlaying())
		{
			FlightAudioComponent->FadeIn(0.1f);
		}
		else
		{
			FlightAudioComponent->Play();
		}
	}
}

void AProjectileBase::PlayImpactSound()
{
	if(ProjectileImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ProjectileImpactSound, GetActorLocation(), GetActorRotation());
	}
}

bool AProjectileBase::ProjectileInteract_Implementation(AActor* InInteractingActor)
{
	if(!InInteractingActor)
	{
		return false;
	}

	// Ignore any friendly pawn, if bDamageOnlyHostilePawns is true
	if(bDamageOnlyHostilePawns)
	{
		if(APawn* InteractingPawn = Cast<APawn>(InInteractingActor))
		{
			const bool bOtherPawnHostile = UFPFunctionLibrary::IsPawnHostile(GetOwner<APawn>(), InteractingPawn);
			if(!bOtherPawnHostile)
			{
				return false;
			}
		}
	}

	// Apply damage to the actor, if it implements IDamageableInterface  
	if(InInteractingActor->Implements<UDamageableInterface>())
	{
		AController* OwningController = GetOwner<APawn>() ? GetOwner<APawn>()->GetController() : nullptr;
		
		IDamageableInterface::Execute_TakeDamage(InInteractingActor, this, DamageValue, OwningController);
	}

	return true;
}

void AProjectileBase::OnProjectileHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bIgnoreOwner && OtherActor && OtherActor == GetOwner())
	{
		return;
	}
	
	if(ProjectileInteract(OtherActor))
	{
		OnProjectileImpact();
	}


	// Return the projectile to its pool
	SetPooledActorActive(false);
}

void AProjectileBase::OnProjectileBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                              AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                              const FHitResult& SweepResult)
{
	if(bIgnoreOwner && OtherActor && OtherActor == GetOwner())
	{
		return;
	}

	if(ProjectileInteract(OtherActor))
	{
		OnProjectileImpact();
	}
	
	// Return the projectile to its pool, if it can't pierce actors 
	if(!bPierceActors)
	{
		SetPooledActorActive(false);
	}
}


void AProjectileBase::OnProjectileImpact_Implementation()
{
	PlayImpactSound();

	if(ImpactNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactNiagaraSystem,
			GetActorLocation(),
			GetActorRotation()
			);
	}
}
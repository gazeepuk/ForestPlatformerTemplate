// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "CoreTypes/FPCustomCollisions.h"
#include "FunctionLibrary/FPFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "Kismet/GameplayStatics.h"


AProjectileBase::AProjectileBase()
{
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionObjectType(ECC_FP_Projectile_OC);
	ProjectileCollision->SetCollisionResponseToChannel(ECC_FP_Projectile_OC, ECR_Ignore);
	ProjectileCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
	ProjectileCollision->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	SetRootComponent(ProjectileCollision);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetActive(false);

	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FlightAudioComponent"));
	FlightAudioComponent->SetupAttachment(GetRootComponent());
	FlightAudioComponent->bAutoActivate = false;
	FlightAudioComponent->bAllowSpatialization = true;
}

void AProjectileBase::LaunchProjectile(FVector InLocation, FVector InDirection, float InInitialSpeed, float InMaxSpeed,
	float InGravityScale)
{

	SetActorLocationAndRotation(InLocation, InDirection.Rotation());

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

void AProjectileBase::ActivateActor_Implementation()
{
	Super::ActivateActor_Implementation();

	ProjectileMovementComponent->SetActive(true);

	PlayFlightSound();
}

void AProjectileBase::DeactivateActor_Implementation()
{
	FlightAudioComponent->Stop();
	FlightAudioComponent->SetSound(nullptr);

	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->StopMovementImmediately();
	
	ProjectileMovementComponent->SetActive(false);

	Super::DeactivateActor_Implementation();
}

void AProjectileBase::PlayFlightSound()
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

void AProjectileBase::OnDeactivateProjectile_Implementation()
{
	SetPooledActorActive(false);
}

bool AProjectileBase::ProjectileInteract_Implementation(AActor* InInteractingActor)
{
	if(!InInteractingActor)
	{
		return false;
	}

	if(APawn* InteractingPawn = Cast<APawn>(InInteractingActor))
	{
		const bool bOtherPawnHostile = UFPFunctionLibrary::IsPawnHostile(GetOwner<APawn>(), InteractingPawn);
		if(bDamageOnlyHostilePawns && !bOtherPawnHostile)
		{
			return false;
		}
	}

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
	
	ProjectileInteract(OtherActor);

	OnProjectileImpact();
	
	OnDeactivateProjectile();
}

void AProjectileBase::OnProjectileBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                              AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                              const FHitResult& SweepResult)
{
	if(bIgnoreOwner && OtherActor && OtherActor == GetOwner())
	{
		return;
	}

	ProjectileInteract(OtherActor);

	OnProjectileImpact();
	
	if(!bPierceActors)
	{
		OnDeactivateProjectile();
	}
}


void AProjectileBase::OnProjectileImpact_Implementation()
{
	PlayImpactSound();
}
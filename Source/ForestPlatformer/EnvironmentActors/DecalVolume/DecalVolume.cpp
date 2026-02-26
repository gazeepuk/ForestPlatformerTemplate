// Developed by Ivan Piankouski. All Rights Reserved


#include "DecalVolume.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"


ADecalVolume::ADecalVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(GetRootComponent());
	DecalComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	MatchDecalSize();
}

void ADecalVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	MatchDecalSize();
}

void ADecalVolume::MatchDecalSize() const
{
	if (BoxComponent && DecalComponent)
	{
		const FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
		DecalComponent->DecalSize = FVector(BoxExtent.Z, BoxExtent.Y, BoxExtent.X);
	}
}

#if WITH_EDITOR
void ADecalVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	MatchDecalSize();
}
#endif

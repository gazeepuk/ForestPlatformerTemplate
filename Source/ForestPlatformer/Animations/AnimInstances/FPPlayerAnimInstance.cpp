// Developed by Ivan Piankouski. All Rights Reserved


#include "FPPlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UFPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(OwningMovementComponent)
	{
		bShouldMove = !bFalling && GroundSpeed > 3.f && !OwningMovementComponent->GetCurrentAcceleration().IsNearlyZero();
	}
}

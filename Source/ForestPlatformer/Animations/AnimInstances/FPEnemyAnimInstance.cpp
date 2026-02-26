// Developed by Ivan Piankouski. All Rights Reserved


#include "FPEnemyAnimInstance.h"

void UFPEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	bShouldMove = GroundSpeed > 3.f && !bFalling;
}

#include "FPGameplayTags.h"

namespace FPGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Invincible, "Shared.Status.Invincible")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Strafing, "Shared.Status.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_UnderAttack, "Shared.Status.UnderAttack")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Attacking, "Shared.Status.Attacking")

	// Attack Tags
	UE_DEFINE_GAMEPLAY_TAG(Attack_Player_Seed_Melee, "Attack.Player.Seed.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Attack_Player_Seed_Ranged, "Attack.Player.Seed.Ranged")
	
	UE_DEFINE_GAMEPLAY_TAG(Attack_Enemy_Mushroom_Melee, "Attack.Enemy.Mushroom.Melee")
	
	UE_DEFINE_GAMEPLAY_TAG(Attack_Enemy_Fly_Melee, "Attack.Enemy.Fly.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Attack_Enemy_Fly_Range, "Attack.Enemy.Fly.Range")
	
}

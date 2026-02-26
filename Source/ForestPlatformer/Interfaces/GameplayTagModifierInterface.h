// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagModifierInterface.generated.h"

struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE()
class UGameplayTagModifierInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for modifying tag containers at runtime.
 * The interface is designed to be implemented by any actor that needs to expose tag management functionality.
 * It's primarily used by UFPFunctionLibrary helper functions, which rely on this interface.
 */
class FORESTPLATFORMER_API IGameplayTagModifierInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Adds the specified gameplay tag to a gameplay tag container */
	virtual void AddGameplayTag(const FGameplayTag& InGameplayTag) = 0;
	/** Removes the specified gameplay tag from a gameplay tag container */
	virtual void RemoveGameplayTag(const FGameplayTag& InGameplayTag) = 0;
};

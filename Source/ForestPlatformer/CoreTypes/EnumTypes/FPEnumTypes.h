// Developed by Ivan Piankouski. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EFPConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EFPSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EFPComparisonType : uint8
{
	LessThan,
	EqualsTo,
	GreaterThan,
	NotEqual
};

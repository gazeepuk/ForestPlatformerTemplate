// Fill out your copyright notice in the Description page of Project Settings.

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

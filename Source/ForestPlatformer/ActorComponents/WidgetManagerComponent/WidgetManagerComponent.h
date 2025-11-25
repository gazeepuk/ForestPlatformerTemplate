// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetManagerComponent.generated.h"

struct FInputActionInstance;
struct FInputActionValue;

UENUM()
enum class EInputMode
{
	GameOnly	UMETA(DisplayName = "Game Only"),
	UIOnly		UMETA(DisplayName = "UI Only"),
	GameAndUI	UMETA(DisplayName = "Game and UI")
};

USTRUCT(BlueprintType)
struct FFPInputMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputMode InputMode;
};

class UInputMappingContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORESTPLATFORMER_API UWidgetManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void OpenWidgetByClass(TSubclassOf<UUserWidget> InWidgetClass);
	UFUNCTION(BlueprintCallable)
	void CloseTopWidget();
	UFUNCTION(BlueprintPure)
	bool IsWidgetOpen(TSubclassOf<UUserWidget> InWidgetClass);

protected:
	
	UFUNCTION(BlueprintNativeEvent)
	APlayerController* GetPLayerController() const;
	
	TArray<UUserWidget*> OpenedWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> UIMappingContext;
	
	UPROPERTY(EditAnywhere)
	int32 UIContextPriority = 1;

	UPROPERTY(EditAnywhere)
	FFPInputMode GameInputMode;

	UPROPERTY(EditAnywhere)
	FFPInputMode UIInputMode;
	
private:
	void HandleUIMappingContext() const;
	
	void AddUIContext() const;
	void RemoveUIContext() const;

	void SetPCInputMode(FFPInputMode InputMode) const;
};

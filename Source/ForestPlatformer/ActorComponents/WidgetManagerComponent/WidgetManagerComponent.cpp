// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManagerComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"


void UWidgetManagerComponent::OpenWidgetByClass(TSubclassOf<UUserWidget> InWidgetClass)
{
	if(!InWidgetClass)
	{
		return;	
	}
	
	if(IsWidgetOpen(InWidgetClass))
	{
		return;
	}

	if(UUserWidget* UserWidget = CreateWidget(GetPLayerController(), InWidgetClass))
	{
		UserWidget->AddToViewport();
		OpenedWidgets.AddUnique(UserWidget);
	}

	HandleUIMappingContext();
}

void UWidgetManagerComponent::CloseTopWidget()
{
	if(OpenedWidgets.IsEmpty())
	{
		return;
	}
	
	if(UUserWidget* LastWidget = OpenedWidgets.Last())
	{
		LastWidget->RemoveFromParent();
		OpenedWidgets.Remove(LastWidget);
	}

	HandleUIMappingContext();
}

bool UWidgetManagerComponent::IsWidgetOpen(TSubclassOf<UUserWidget> InWidgetClass)
{
	if(!InWidgetClass)
	{
		return false;
	}

	UUserWidget** OpenedWidget = OpenedWidgets.FindByPredicate(
	[&InWidgetClass](const UUserWidget* Widget)
	{
		return Widget && Widget->GetClass() == InWidgetClass;
	});

	return OpenedWidget && *OpenedWidget;
}

void UWidgetManagerComponent::HandleUIMappingContext() const
{
	if(OpenedWidgets.IsEmpty())
	{
		RemoveUIContext();
		SetPCInputMode(GameInputMode);
	}
	else
	{
		AddUIContext();
		SetPCInputMode(UIInputMode);
	}
}

APlayerController* UWidgetManagerComponent::GetPLayerController_Implementation() const
{
	return GetOwner<APlayerController>();
}

void UWidgetManagerComponent::AddUIContext() const
{
	if(const APlayerController* PC = GetPLayerController())
	{
		if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if(!EnhancedInputSubsystem->HasMappingContext(UIMappingContext))
			{
				EnhancedInputSubsystem->AddMappingContext(UIMappingContext, UIContextPriority);
			}
		}
	}
}

void UWidgetManagerComponent::RemoveUIContext() const
{
	if(const APlayerController* PC = GetPLayerController())
	{
		if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if(EnhancedInputSubsystem->HasMappingContext(UIMappingContext))
			{
				EnhancedInputSubsystem->RemoveMappingContext(UIMappingContext);
			}
		}
	}
}

void UWidgetManagerComponent::SetPCInputMode(FFPInputMode InputMode) const
{
	if(APlayerController* PC = GetPLayerController())
	{
		PC->SetShowMouseCursor(InputMode.bShowCursor);
		switch (InputMode.InputMode)
		{
		case EInputMode::GameOnly:
			PC->SetInputMode(FInputModeGameOnly());
			break;
		case EInputMode::UIOnly:
			PC->SetInputMode(FInputModeUIOnly());
			break;
		case EInputMode::GameAndUI:
			PC->SetInputMode(FInputModeGameAndUI());
			break;
		}
	}
}

// Copyright Grégoire Lebas


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
                           UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized."));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized."))
	
	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	UOverlayWidgetController* OverlayController = GetOverlayWidgetController(Params);

	UAuraUserWidget* AuraUserWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	AuraUserWidget->SetWidgetController(OverlayController);

	OverlayWidgetController->BroadcastInitialValues();
	
	AuraUserWidget->AddToViewport();
}

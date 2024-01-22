// Copyright Unreal Robin


#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

/* Constructs an Overlay Widget Controller
 * if one hasn't been constructed already
 */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		/* Creating a new Widget Controller */
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Class uninitialized! Please fill out BP_AuraHUD!"));
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized! Please fill out BP_AuraHUD!"));

	/* Creating a new Widget of Parent Class OverlayWidget and casting to UAuraUserWidget*/
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);


	/* Constructing the Widget Controller if not already created.*/
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	/* Setting the Overlay Widgets Controller to Widget Controller*/
	OverlayWidget->SetWidgetController(WidgetController);
	
	Widget->AddToViewport();
}



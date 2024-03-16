// Copyright Unreal Robin


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

/* Constructs an Overlay Widget Controller
 * if one hasn't been constructed already
 */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		/* Creating a new Widget Controller */
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		//Setting the Widget Controller Params
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		//Binding the Callbacks to the Dependencies
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		//Creating a new Object of type UAttributeMenuWidgetController
		//The outer object in this case is the AuraHUD, it is optional, but because we want to set the class of the new object to
		//AttributeMenuWidgetControllerClass, we need to
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
		return AttributeMenuWidgetController;
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Class uninitialized! Please fill out BP_AuraHUD!"));
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized! Please fill out BP_AuraHUD!"));

	/* Creating a new Widget of Parent Class OverlayWidget and casting to UAuraUserWidget*/
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);


	/* Constructing the Widget Controller if not already created.
	 * Constructing the Struct to be passed to the Widget Controller.
	 * Data is recieved from the AuraCharacter class during InitAbilityActorInfo()
	 */
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	/* Setting the Overlay Widgets Controller to Widget Controller*/
	OverlayWidget->SetWidgetController(WidgetController);

	//Broadcasting the Initial Values
	WidgetController->BroadcastInitialValue();

	Widget->AddToViewport();
}

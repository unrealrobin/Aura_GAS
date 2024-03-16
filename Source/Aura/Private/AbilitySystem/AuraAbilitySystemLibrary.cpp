// Copyright Unreal Robin


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(UObject* WorldContextObject)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC)
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAttributeSet* AS = PS->GetAttributeSet();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	//We first needs to get the Player Controller in order to  get the HUD set on the player controller
	// The player controller is also easily accessible from the World Context Object
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	//Retrieve Index 0 Players HUD
	if (PC)
	{
		//Storing HUD in a variable
		AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (AuraHUD)
		{
			// Variables we need to set Params for the Widget Controller
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAttributeSet* AS = PS->GetAttributeSet();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();

			//Creating the WidgetControllerParams Object that is defined in AuraHUD.h
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			//Calling the GetAttributeMenuWidgetController function from the AuraHUD
			//This function will create a new Widget Controller if it hasn't been created already
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

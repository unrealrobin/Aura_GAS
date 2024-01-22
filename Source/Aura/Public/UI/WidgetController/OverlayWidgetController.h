// Copyright Unreal Robin

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, MaxHealth);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValue() override;

	/* These Delegates are Called when there is a change with their associated Values
	 * FOnHealthChangedSignature is a Type we created to be called as a delegate
	 * We then create a variable of that type and call it OnHealthChanged
	 * Delegates are called by using the .Broadcast() function
	 */
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnHealthChangedSignature OnMaxHealthChanged;

};

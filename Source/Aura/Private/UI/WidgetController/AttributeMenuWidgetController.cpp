// Copyright Unreal Robin


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	/*
	 * We are looping through all the attributes in our AttributeSet and binding a delegate to the attribute change event.
	 * We need to bind this, Pair, and AS to the lambda function because we are using them inside the lambda function.
	 * Specifically, Pair and AS because of scoping issues, so we are capturing them by value.
	 * When an attributes value changes, Pair.Value(), we create a new AuraAttributeInfo struct, populate it with the attribute value, and broadcast it to the UI.
	 * This happens for all the attributes in our AttributeSet.
	 */
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for(auto& Pair: AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
	[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	
}

void UAttributeMenuWidgetController::BroadcastInitialValue() //Called In Blueprints
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	// We are checking if the AttributeInfo Data Asset is valid
	check(AttributeInfo);

	
	// Loops through all the attributes in our TagsToAttributes map and broadcasts the initial value of the attribute to the UI
	for(auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet); //Returns a Gameplay Attribute 
	AttributeInfoDelegate.Broadcast(Info);
}

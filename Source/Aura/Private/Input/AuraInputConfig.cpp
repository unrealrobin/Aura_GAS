// Copyright Unreal Robin


#include "Input/AuraInputConfig.h"

// This function is used to find the input action for a given tag
const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(const FAuraInputAction& Action : AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.MatchesTag(InputTag))
		{
			return Action.InputAction;
		}
	}

	if(!bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("No input action found for tag %s"), *InputTag.ToString());
	}

	return nullptr;
	
}

// Copyright Unreal Robin

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * Default Input Component
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/* A Template Function that is capable of recieving function ptrs */
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

// Template Function Definitions are always in the header file
// This function will give each of our Input Actions (the ability buttons we press) a function to call when they are pressed, released, or held
// We bind this function in our PlayerController and Set the InputConfig in BP_AuraPlayerController
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
                                             PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                             HeldFuncType HeldFunc)
{
	check(InputConfig);

	//Binding TriggerEvents for each of our InputActions
	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (HeldFunc)
			{
				// The Triggered event is called every frame the action is held down
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}

			if (PressedFunc)
			{
				// The Started Event is called when the action is first pressed
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				// The Completed Event is called when the action is released
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}

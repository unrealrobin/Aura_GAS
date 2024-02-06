// Copyright Unreal Robin


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/* Why do we call this here?
	 *
	 * So each of our characters needs to be able to call this function.
	 * Each character calls this function during their AbilitySystemSetup. Its called through another function
	 * called InitAbilityActorInfo. This function is called in the PossessedBy function in the AuraCharacter.cpp file. This function
	 * is called on each characters AbilitySystemComponent.
	 *
	 * What this function does is it binds a delegate to the OnGameplayEffectAppliedDelegateToSelf.
	 * OnGameplayEffectAppliedDelegateToSelf is a delegate that is called whenever a gameplay effect is applied to the character.
	 * When we bind to a delegate we are essentially saying "Hey, when this event happens, call this function".
	 *
	 * The function we are calling is in response to an EffectApplied event. This function is called EffectApplied.
	 *
	 * This Delegate was created in the UAbilitySystemComponent by Epic Games. We are just binding to it.
	 * But Delegates have certain params that need to be passed to them. So the function we are calling needs to have the same
	 * params. You can find the params in the documentation for the delegate.
	 */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	/* Tag Container stores all of our Tags*/
	FGameplayTagContainer TagContainer;
	/* GetAllAssetTags() retrieves all the asset tags*/
	EffectSpec.GetAllAssetTags(TagContainer);
	
	/* Broadcasting lets all the binded functions to know to run
	 * We created a Delegate that gets fired when this function is called.
	 * 
	 */
	EffectAssetTags.Broadcast(TagContainer);
	
	/* Looping through all the tags in our container and doing something with each Tag.*/
	
}

// Copyright Unreal Robin

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * AuraGameplayTags
 *
 * Singleton containing native gameplay tags.
 * Native - Created in C++ and Available in C++ and Blueprints
 * Singleton means that there is only one instance of this class in the game.
 *
 * Singletons - Static class with static members
 */
struct FAuraGameplayTags
{
public:
 static const FAuraGameplayTags& Get(){return GameplayTags;} // Singleton accessor

 static void InitializeNativeGameplayTags();
protected:
 
private:
 static FAuraGameplayTags GameplayTags;
};

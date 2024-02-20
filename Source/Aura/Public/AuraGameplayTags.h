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

 /* Secondary GameplayTags */
 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_ArmorPentration;
 FGameplayTag Attributes_Secondary_MaxMana;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_BlockChance;
 FGameplayTag Attributes_Secondary_CritHitChance;
 FGameplayTag Attributes_Secondary_CritHitDamage;
 FGameplayTag Attributes_Secondary_CritHitResistance;
 FGameplayTag Attributes_Secondary_HealthRegen;
 FGameplayTag Attributes_Secondary_ManaRegen;

 /*Primary GameplayTags*/
 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Intelligence;
 FGameplayTag Attributes_Primary_Vigor;
 FGameplayTag Attributes_Primary_Resilience;


protected:
 
private:
 static FAuraGameplayTags GameplayTags;
};

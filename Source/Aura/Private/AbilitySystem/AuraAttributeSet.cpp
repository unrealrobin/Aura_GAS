// Copyright Unreal Robin


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// ~ This function is automatically created by the Macro ATTRIBUTE_ACCESSORS.
	// ~ The Macro creates a set of Getter and Setter functions for each attribute of the input class.
	// ~ The format of each of the created functions is the following:
	// ~ GetYOURATTRIBUTENAME()
	// ~ SetYOURATTRIBUTENAME()
	// ~ InitYOURATTRIBUTENAME()
	InitHealth(50.f);
	InitMana(50.f);
	InitMaxHealth(100.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	 *	This is the location where we register the attributes to be replicated to clients.
	 * 1. Replicated with No Conditions
	 * 2. Replicating the Health attribute to clients with no conditions.
	 * 3. Replicating Always, even if the value didn't change.
	 *	Can be changed to REPNOTIFY_Always if you want to replicate only when the value changes.
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

// ~ Called when the Health attribute is replicated to clients.
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{

	// ~ Informs the GAS component that the Health attribute has changed.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);

}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

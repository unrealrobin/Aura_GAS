// Copyright Unreal Robin

#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}



void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/* This is a function from Unreal Engines Public Librarys
	 * It returns a target actors Ability System Component
	 */
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;

	check(GameplayEffectClass);

	/*Creates a Gameplay Effect Context
	 * Creates a Handle for the Effect Context
	 * A Handle is a pointer to a object with an Internal Data Member
	 */
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	/*Creating a Effect Spec*/
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);

	/*
	 * ApplyGameplayEffectSpecToSelf takes a CONST EffectSpec
	 * EffectSpecHandle returns a Handle to a EffectSpec
	 * .Data returns the EffectSpec as a pointer
	 * Get() returns the pointer
	 * But Because Get() returns a pointer to a pointer we need to dereference it with *
	 *
	 *
	 *There are also other options for ApplyGameplayEffect  like:
	 * ApplyGameplayEffectSpecToTarget
	 * ApplyGameplayEffectSpecToOwner etc.
	 */
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

}



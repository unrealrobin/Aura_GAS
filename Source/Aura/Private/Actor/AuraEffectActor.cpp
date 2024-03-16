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
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	/*Creates a Gameplay Effect Context
	 * Creates a Handle for the Effect Context
	 * A Handle is a pointer to a object with an Internal Data Member
	 */
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	/*Creating a Effect Spec*/
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, ActorLevel, EffectContextHandle);

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
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	//Returns whether the Effect is infinite or not
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy ==
		EGameplayEffectDurationType::Infinite;

	// We only want to store the Active Effect Handle if the Effect is infinite and the Effect Removal Policy is RemoveOnEndOverlap
	// because we are only keeping track of effects that we want to remove on EndOverlap
	// all other effects are removed automatically
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandlesMap.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		/* After we remove the effect from the actor we want to remove the pairing from the EffectHandlesMap
		 * We will temporarily store the Handles we want to remove in this array
		 */
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		/* Removes the gameplay effect from the matching Actor.
		 * Stores the Handle in the HandlesToRemove array.
		 */
		for (const auto HandlePair : ActiveEffectHandlesMap)
		{
			if (TargetASC == HandlePair.Value)
			{
				// Removes the Effect from the Actor
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				// Stores the Handle in the HandlesToRemove array
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			// Removes the Handle from the Map
			ActiveEffectHandlesMap.FindAndRemoveChecked(Handle);
		}
	}
}

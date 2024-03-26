// Copyright Unreal Robin


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager); //returns a pointer
	return *AssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data
	UAbilitySystemGlobals::Get().InitGlobalData();
}

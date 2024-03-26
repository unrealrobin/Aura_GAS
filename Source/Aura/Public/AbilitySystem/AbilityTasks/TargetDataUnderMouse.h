// Copyright Unreal Robin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, 
DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:

	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (
		DisplayName = "TargetDataUnderMouse",
		HidePin = "OwningAbility", 
		DefaultToSelf = "OwningAbility",
		BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature TargetData;

private:

	virtual void Activate() override;
	void SendMouseCursorData();

	// THis function will be called after the server receives the target data from the client via the delegate.
	void OnTargetDataReplicationCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);
	
};

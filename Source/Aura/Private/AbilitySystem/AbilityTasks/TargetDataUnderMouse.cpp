// Copyright Unreal Robin


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	// If we are on the client, we send the mouse cursor data because the server will not have the cursor data
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//TODO: We are on the server so listen for Target Data
	}

}

void UTargetDataUnderMouse::SendMouseCursorData()
{

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	// Getting the location of the cursor.
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//Create a Data Hanle to store the Hit Result and make transfering data simpler.
	FGameplayAbilityTargetDataHandle DataHandle;
	
	// Create a new object of this Type.
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	// HitResult is a variable in the FGameplayAbilityTargetData_SingleTargetHit struct. We set it equal to the CursorHit.
	Data->HitResult = CursorHit;
	
	// Add the Data to the Data Handle.
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	 if(ShouldBroadcastAbilityTaskDelegates())
	 {
		 TargetData.Broadcast(DataHandle);
	 };
}

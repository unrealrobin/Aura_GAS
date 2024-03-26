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
		/*
		 * The AbilityTargetDataSetDelegate is a delegate that is called when the server receives the target data from the client.
		 * It Returns the Delegate , we can then create a function that will be called when the delegate is called.
		 * We could bind a lambda function to the delegate that will be called when the delegate is called.
		 * Or we could bind a function that is a member of the class that the delegate is in.
		 *
		 * As soon as activate is called on the server, the server will bind the OnTargetDataReplicationCallback function to the delegate.
		 */
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicationCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, 
		ActivationPredictionKey);

		if(!bCalledDelegate)
		{
			// Still waiting on remote player data if we didn't get a callback.
			SetWaitingOnRemotePlayerData();
		}
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

void UTargetDataUnderMouse::OnTargetDataReplicationCallback(
	const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag)
{
	// Will be called on the server when replication data (target data) has been received.

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		TargetData.Broadcast(Data);
	};
}

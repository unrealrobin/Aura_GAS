// Copyright Unreal Robin


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	// Getting the location of the cursor
	// Ability Tasks know about the Ability and the Ability's ActorInfo
	// ActorInfo is a struct that contains the Actor, the Avatar Actor, and the PlayerController
	// We use Get because the the Returned player controller is a TWeakObjectPtr
	// A TWeakObjectPtr is a pointer that can be null, and it is not guaranteed to be valid
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// Broadcasting the location of the cursor
	// Will mean the TargetData Execution Pin will be fired in BP's
	TargetData.Broadcast(CursorHit.Location);
}

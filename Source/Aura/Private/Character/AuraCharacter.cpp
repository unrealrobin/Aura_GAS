// Copyright Unreal Robin


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	/*Init Ability Info for the Server*/
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	/*Init Ability Actor Info for the Client*/
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	
	/*GAS Variables and Initialization*/
	//Caster this Characters Player state to AuraPlayerState and storing in a variable.
	//Using the variable to access the GAS Component and Attribute Set.
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		};
	};
}







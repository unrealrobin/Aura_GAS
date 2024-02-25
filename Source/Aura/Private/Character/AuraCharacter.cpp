// Copyright Unreal Robin


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
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
	
	AddCharacterAbilities(); //Inherited from AuraCharacterBase class
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	/*Init Ability Actor Info for the Client*/
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	// Get this Players PLayerState and Cast it to AuraPlayerState
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	//Calls the AbilityActorInfoSet function in the AuraAbilitySystemComponent
	//We want to call this here because we want to bind the delegates in the AbilityActorInfoSet function
	//This location is the best place to call it because we have all the necessary variables ready at this point.
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	/*GAS Variables and Initialization*/
	//Caster this Characters Player state to AuraPlayerState and storing in a variable.
	//Using the variable to access the GAS Component and Attribute Set.
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	
	AttributeSet = AuraPlayerState->GetAttributeSet();

	/* Turn the Players HUD into our Custom Hud and Spawn the Overlay Widget*/
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			/* We Call InitOverlay Here because we have all the necessary variables
			 * that we want to construct the Widget Controller available at this point.
			 */
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		};
	};

	/*Initialized the Primary & Secondary Attributes, this function is inherited from Parent Class.*/
	InitializeDefaultAttributes();
}







// Copyright Unreal Robin


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Setting a variable to store the AbilitySystemComponent
	//On the Base Character Class we are storing the parent to the UAuraAbilitySystemComponent
	//These Variables are inherited from the Base Character Class
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);//For Multiplayer
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*Initializing the Actor Info for the Enemy. Enemy is the Actor and Avatar*/
	// We dont need to check the ABilitySystemComponent because we are initializing it in the constructor.
	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	/* Ability System Component is Inherited from AuraCharacterBase */
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

}

void AAuraEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//If enemy should be highlighted, draw a debug sphere around it.
	if(ShouldHighlight)
	{
		//DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Blue, true, -1, 0, 2.f);
		//UE_LOG(LogTemp, Warning, TEXT("Enemy is Highlighted in tick"));
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

		//Weapon is from the AuraCharacterBase class.
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enemy is UnHighlighted in tick"));
		GetMesh()->SetRenderCustomDepth(false);
		Weapon->SetRenderCustomDepth(false);
		
	}
	
	
}

void AAuraEnemy::HighlightActor()
{
	ShouldHighlight = true;
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Highlight Variable Updated to true"));
}

void AAuraEnemy::UnHighlightActor()
{
	ShouldHighlight = false;
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Highlight Variable Updated to false"));

}



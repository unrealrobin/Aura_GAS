// Copyright Unreal Robin


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
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



// Copyright Unreal Robin


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	LastActor = nullptr;
	CurrentActor = nullptr;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	
}

void AAuraPlayerController::AutoRun()
{
	// If AutoRunning is False, return. This is set to True in the Released Function after we have found a path to follow.
	if(!bAutoRunning) return;
	//This is the code that makes the player character move along the spline.
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	};
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//Check() is an Unreal Engine function that checks if the object is valid.
	check(AuraContext);

	//Connecting the input mapping context to the local player subsystem.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	//these are the default values for the mouse cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//The input mode is set to game and UI, which means that the mouse cursor is visible and can be moved around.
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	
	//player controller has an input component variable that is used to bind actions and axis mappings to functions.
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	// The order of the arguments is important.
	// Binding Our input actions to our functions
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//this is the function that is called when the player presses the move button.
	//the input action value is a struct that contains the value of the input action.
	//in this case, the value is a vector2d, which is a 2d vector.
	//the vector2d is the direction that the player wants to move in.
	//the vector2d is normalized, which means that the length of the vector is 1.
	//this is useful because it means that the player will move at the same speed in all directions.
	//the vector2d is multiplied by the move speed, which is a float variable that can be changed in the editor.
	//the move speed is multiplied by the delta time, which is the time between frames.
	//this is useful because it means that the player will move at the same speed regardless of the frame rate.
	//the move vector is then added to the player's current location.

	
	//returns the value of the input action as a vector2d.
	FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit)
	{
		return;
	}

	

	//stores what the actor was before we updated it.W
	LastActor = CurrentActor;
	//if Cast succeeds the actor implements the interface
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	
	/*
	* Line trace from Cursor. There are several Scenarios
	*
	* A. Last Actor and Current Actor are both Nullptr
	* 	1. Do nothing
	* B. Last Actor is Null and Current Actor is Valid
	*	1. Call HighlightActor on Current Actor
	* C. Last Actor is Valid and Current Actor is Null
	*	1. Call UnHighlightActor on Last Actor
	* D. Last Actor is Valid and Current Actor is Valid
	*	1. Check if Last Actor is the same as Current Actor
	*	2. If they are the same, do nothing
	*	3. If they are different, call UnHighlightActor on Last Actor and HighlightActor on Current Actor
	*/

	if(LastActor != CurrentActor)
	{
		if(LastActor) LastActor -> UnHighlightActor();
		if(CurrentActor) CurrentActor -> HighlightActor();
		
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//If we are targeting a valid actor, we want to use the ability on that actor. Otherwise, we want to move.
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// If the Input is not the LMB 
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	// If we are targeting, we want to use the ability on the actor we are targeting.
	if(bTargeting)
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else // If we are not targeting, we want to move.
	{
		APawn* ControlledPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// we want to find a path to follow
			//Returns a UNavigationPath
			if(UNavigationPath* NavPath =  UNavigationSystemV1::FindPathToLocationSynchronously(this,
				ControlledPawn->GetActorLocation(),
				CachedDestination))
			{
				//Clearing existing spline points
				Spline->ClearSplinePoints();
				
				//PathPoints is a TArray of FNavPathPoint
				// For each point in the path, we want to add a spline point and draw a debug sphere
				// The points are generated by the Navigation System
				for(const FVector& PointLocation : NavPath->PathPoints)
				{
					//Adding a point to the spline
					Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
				}
				// We want to ignore that last path point
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				// When we create the Path we want to toggle our AutoRunning to true
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//if I am not holding LMB, I want to call the ability input tag held function on the ASC
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	//If we are targeting, we want to use the ability on the actor we are targeting.
	if(bTargeting)
	{
		if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		//Here we are holding LMB and we are not targeting, so we want to move.
		FollowTime += GetWorld()->GetDeltaSeconds();
		if(CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if(APawn* ControlledPawn = GetPawn())
		{
			//Vector from ControlledPawn to the CachedDestination as a Normalized Vector
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
		
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
		
	}

	return AuraAbilitySystemComponent;
}




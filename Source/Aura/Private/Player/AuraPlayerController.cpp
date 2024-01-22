// Copyright Unreal Robin


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SWarningOrErrorBox.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	LastActor = nullptr;
	CurrentActor = nullptr;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//perform trace and handle highlighting
	CursorTrace();
	//UE_LOG(LogTemp, Warning, TEXT("Player Tick"));
	
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
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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
	FHitResult CursorHit;
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

	if (LastActor == nullptr)
	{
		if(CurrentActor != nullptr)
		{
			//Case B
			CurrentActor->HighlightActor();
			UE_LOG(LogTemp, Warning, TEXT("Highlighting Actor"));
		}
		else
		{
			//Case A - Both are Null, Do Nothing
			return;
		}
	}
	else //Last Actor is Valid
	{
		if(CurrentActor == nullptr)
		{
			//Case C
			LastActor->UnHighlightActor();
		}
		else
		{
			if(LastActor != CurrentActor)
			{
				//Case D
				LastActor->UnHighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				//Case D - Same Actor, Do Nothing
				return;
			}
		}
	}
	
}


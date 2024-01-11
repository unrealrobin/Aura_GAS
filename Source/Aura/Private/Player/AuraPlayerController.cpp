// Copyright Unreal Robin


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//Check() is an Unreal Engine function that checks if the object is valid.
	check(AuraContext);

	//Connecting the input mapping context to the local player subsystem.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	//these are the default values for the mouse cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

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

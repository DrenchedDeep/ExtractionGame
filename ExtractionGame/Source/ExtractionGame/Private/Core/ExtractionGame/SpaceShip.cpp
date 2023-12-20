


#include "Core/ExtractionGame/SpaceShip.h"

#include "EnhancedInputComponent.h"


void ASpaceShip::OnDirectionChanged(const FInputActionValue& Value)
{
	const FVector2D prv = Value.Get<FVector2D>();
	if(movementDirection == prv) return;
	
	movementDirection =  prv;
	if(movementDirection.X > 0) // GOING RIGHT, DISABLE RIGHT SIDE
	{
		FrontLeft->Activate();
		BackLeft->Activate();

		FrontRight->Deactivate();
		BackRight->Deactivate();
	}
	else if(movementDirection.X < 0) // LEFT IS OPPOSITE
	{
		FrontLeft->Deactivate();
		BackLeft->Deactivate();
		
		FrontRight->Activate();
		BackRight->Activate();
	}
	if(movementDirection.Y > 0) // GOING UP, DISABLE ALL BACK 
	{
		FrontLeft->Activate();
		FrontMid->Activate();
		FrontRight->Activate();

		BackLeft->Deactivate();
		BackMid->Deactivate();
		BackRight->Deactivate();
	}
	else if(movementDirection.Y < 0) // invs
	{
		FrontLeft->Deactivate();
		FrontMid->Deactivate();
		FrontRight->Deactivate();

		BackLeft->Activate();
		BackMid->Activate();
		BackRight->Activate();
	}
}

void ASpaceShip::OnDirectionStopped()
{
	UE_LOG(LogTemp, Warning, TEXT("MOVEMENT STOPPED"))
	movementDirection = FVector2D::Zero();
	FrontLeft->Deactivate();
	FrontMid->Deactivate();
	FrontRight->Deactivate();
	BackLeft->Deactivate();
	BackMid->Deactivate();
	BackRight->Deactivate();
	
}

void ASpaceShip::StartBoost()
{
	currentSpeed = boostSpeed;
	Primary->Activate();
}

void ASpaceShip::StopBoost()
{
	currentSpeed = regularSpeed;
	Primary->Deactivate();
}

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	StopBoost();
}

void ASpaceShip::OnLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//LocalVerticalLook = FirstPersonCameraComponent->GetComponentRotation().Pitch;
		//LocalHorizontalLook = LookAxisVector.X;

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TODO: On crash disable tick.
	if(!movementDirection.IsNearlyZero())
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += rotationSpeed * movementDirection.X * GetWorld()->GetDeltaSeconds();
		NewRotation.Roll += rotationSpeed * movementDirection.Y * GetWorld()->GetDeltaSeconds();
		SetActorRotation(NewRotation);
	}
	AddMovementInput(GetActorForwardVector(), currentSpeed);
}


// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &ASpaceShip::OnLook);
		EnhancedInputComponent->BindAction(Thrust, ETriggerEvent::Started, this, &ASpaceShip::StartBoost);
		EnhancedInputComponent->BindAction(Thrust, ETriggerEvent::Canceled, this, &ASpaceShip::StopBoost);
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &ASpaceShip::OnDirectionChanged);
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Completed, this, &ASpaceShip::OnDirectionStopped);
	}
}


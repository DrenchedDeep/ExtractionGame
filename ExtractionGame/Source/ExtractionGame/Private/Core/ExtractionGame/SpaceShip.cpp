


#include "Core/ExtractionGame/SpaceShip.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


void ASpaceShip::OnDirectionChanged(const FInputActionValue& Value)
{
	const FVector2D prv = Value.Get<FVector2D>();
	
	if(movementDirection == prv || isCrashed) return;
	UE_LOG(LogTemp, Warning, TEXT("SETTING MOVEMENT DIRECTION: %f, %f"), prv.X, prv.Y)
	
	movementDirection =  prv;

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
	
	if(movementDirection.X < 0) // GOING RIGHT, DISABLE RIGHT SIDE
	{
		FrontLeft->Activate();
		BackLeft->Activate();

		FrontRight->Deactivate();
		BackRight->Deactivate();
	}
	else if(movementDirection.X > 0) // LEFT IS OPPOSITE
	{
		FrontLeft->Deactivate();
		BackLeft->Deactivate();
		
		FrontRight->Activate();
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
	if(isCrashed) return;
	currentSpeed = boostSpeed;
	Primary->Activate();
}

void ASpaceShip::StopBoost()
{
	currentSpeed = regularSpeed;
	Primary->Deactivate();
}

// Sets default values
ASpaceShip::ASpaceShip(): Move(nullptr), Look(nullptr), Thrust(nullptr), ShipMappingContext(nullptr), curRot(0),
                          isCrashed(false),
                          Primary(nullptr),
                          FrontLeft(nullptr),
                          FrontMid(nullptr),
                          FrontRight(nullptr),
                          BackLeft(nullptr),
                          BackMid(nullptr),
                          BackRight(nullptr), FirstPersonCameraComponent(nullptr),
                          regularSpeed(0), boostSpeed(0),
                          rotationSpeed(0), camPanSpeed(0), MaxPitch(0),
                          currentSpeed(0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(ShipMappingContext, 0);
		}
	}
	OnDirectionStopped();
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
	const float dt = GetWorld()->DeltaTimeSeconds;
	const FVector newLocation = GetActorLocation() + GetActorUpVector() * (-currentSpeed * dt);
	FHitResult hit;
	if(!movementDirection.IsNearlyZero())
	{

		FRotator r = GetActorRotation();
		
		const float yaw = movementDirection.X * rotationSpeed * dt;
		const float pitch = movementDirection.Y * rotationSpeed * dt;
		
		const float prv = r.Pitch;
		r.Pitch += pitch;
		
		if(r.Pitch > 0 || r.Pitch < -MaxPitch) r.Pitch = prv;
		else AddControllerPitchInput(pitch/camPanSpeed);
		AddControllerYawInput(yaw/camPanSpeed);
		
		//AddActorLocalRotation(FRotator(pitch, 0, 0));

		curRot += yaw;
		
		//r.Pitch = FMath::Clamp(r.Pitch,-MaxPitch,0);
		
		
		SetActorLocationAndRotation(newLocation, FQuat(FVector::UnitZ(), FMath::DegreesToRadians(yaw)).Rotator() + r, true, &hit);
	}
	else
	{
		SetActorLocation(newLocation, true, &hit);
	}
	

	if(hit.IsValidBlockingHit())
	{
		StopBoost();
		OnDirectionStopped();
		//SetActorTickEnabled(false);
		isCrashed = true;
		CrashLand(hit);
	}
	
	//AddMovementInput(GetActorForwardVector(), currentSpeed);
}


// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &ASpaceShip::OnLook);
		EnhancedInputComponent->BindAction(Thrust, ETriggerEvent::Started, this, &ASpaceShip::StartBoost);
		EnhancedInputComponent->BindAction(Thrust, ETriggerEvent::Completed, this, &ASpaceShip::StopBoost);
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &ASpaceShip::OnDirectionChanged);
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Completed, this, &ASpaceShip::OnDirectionStopped);
	}
}


#include "ExtractionGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AExtractionGameCharacter::AExtractionGameCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AExtractionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void AExtractionGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);



	if(IsLocallyControlled())
	{
		GEngine->AddOnScreenDebugMessage(-10, 1.f, FColor::Yellow, bCrouchPressed ? "True" : "False");

		//predict it
		IsSprinting = bSprintPressed;
		IsCrouching = bCrouchPressed;

		//send values to server
		Server_SendInput(LocalVerticalMovement, LocalHorizontalMovement, bSprintPressed, bCrouchPressed);

		bSprintPressed = false;
	}

	if(HasAuthority() || IsLocallyControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetMovementSpeed();

		if(IsLocallyControlled())
		{
			const FVector ToLocation = IsCrouching ? CrouchCameraLocation : DefaultCameraLocation;
			const FVector CameraLocation = FMath::Lerp(FirstPersonCameraComponent->GetRelativeLocation(), ToLocation,DeltaSeconds * CrouchCameraOffsetSmoothing);
			
			FirstPersonCameraComponent->SetRelativeLocation(CameraLocation);
		}

	
		if(IsSprinting && IsCrouching)
		{
			
		}
	}
}

void AExtractionGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Look);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::StartSprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::None, this, &AExtractionGameCharacter::StopSprintPressed);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::CrouchPressed);
	}
}

void AExtractionGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AExtractionGameCharacter, IsSprinting);
	DOREPLIFETIME(AExtractionGameCharacter, IsCrouching);
	DOREPLIFETIME(AExtractionGameCharacter, IsSliding);
	DOREPLIFETIME(AExtractionGameCharacter, VerticalMovement);
	DOREPLIFETIME(AExtractionGameCharacter, HorizontalMovement);
}

float AExtractionGameCharacter::GetMovementSpeed() const
{
	float Speed = WalkMovementSpeed;

	if(IsSprinting)
	{
		Speed = SprintMovementSpeed;
	}

	if(IsCrouching)
	{
		Speed = CrouchMovementSpeed;
	}

	return Speed;
}


void AExtractionGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AExtractionGameCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AExtractionGameCharacter::StartSprintPressed()
{
	bSprintPressed = true;
}

void AExtractionGameCharacter::StopSprintPressed()
{
	bSprintPressed = false;
}

void AExtractionGameCharacter::CrouchPressed()
{
	bCrouchPressed = !bCrouchPressed;
}

void AExtractionGameCharacter::Server_SendInput_Implementation(float LocVerticalMovement, float LocHorizontalMovement,
	bool LocSprintPressed, bool LocCrouchPressed)
{
	VerticalMovement = LocVerticalMovement;
	HorizontalMovement = LocHorizontalMovement;

	IsSprinting = LocSprintPressed;
	IsCrouching = LocCrouchPressed;
}

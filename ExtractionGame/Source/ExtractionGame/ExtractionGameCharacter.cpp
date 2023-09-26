#include "ExtractionGameCharacter.h"

#include <filesystem>

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerHealthComponent.h"
#include "PlayerMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AExtractionGameCharacter::AExtractionGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetCharacterMovement());
	PlayerHealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("HealthComponent"));
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

	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetCharacterMovement());

}

void AExtractionGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsLocallyControlled())
	{
		Server_SetInput(LocalVerticalMovement, LocalHorizontalMovement, LocalVerticalLook, LocalHorizontalLook);
	}
}

void AExtractionGameCharacter::Server_SetInput_Implementation(float VerticalMove, float HorizontalMove,
                                                              float VertLook, float HorLook)
{
	VerticalMovement = FMathf::Lerp(VerticalMovement, VerticalMove, GetWorld()->GetDeltaSeconds() * 5.f);
	HorizontalMovement = FMathf::Lerp(HorizontalMovement, HorizontalMove, GetWorld()->GetDeltaSeconds() * 5.f);

	VerticalLook = VertLook;
	HorizontalLook = HorLook;
}

void AExtractionGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::None, this, &AExtractionGameCharacter::ResetMove);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Look);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::SprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::SprintReleased);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::CrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::CrouchReleased);

		EnhancedInputComponent->BindAction(LeftAttackAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::LeftFirePressed);
		EnhancedInputComponent->BindAction(LeftAttackAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::LeftFireReleased);
		
		EnhancedInputComponent->BindAction(RightAttackAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::RightFirePressed);
		EnhancedInputComponent->BindAction(RightAttackAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::RightFireReleased);
	}
}

void AExtractionGameCharacter::ResetMove()
{
	LocalVerticalMovement = 0.0f;
	LocalHorizontalMovement = 0.0f;
}


void AExtractionGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AExtractionGameCharacter, VerticalMovement);
	DOREPLIFETIME(AExtractionGameCharacter, HorizontalMovement);
	DOREPLIFETIME(AExtractionGameCharacter, IsSliding);
	DOREPLIFETIME(AExtractionGameCharacter, IsSprinting);
	DOREPLIFETIME(AExtractionGameCharacter, SlideTimer);
}

FCollisionQueryParams AExtractionGameCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}


void AExtractionGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	//bad solution, clients can get rid of issliding check and move while sliding (do we want the player to move while sliding??)
	if (Controller != nullptr && !IsSliding)
	{
		LocalVerticalMovement = MovementVector.Y;
		LocalHorizontalMovement = MovementVector.X;

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AExtractionGameCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		LocalVerticalLook = LookAxisVector.Y;
		LocalHorizontalLook = LookAxisVector.X;

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AExtractionGameCharacter::SprintPressed()
{
	if(PlayerMovementComponent == nullptr)
	{
		return;
	}
	
	PlayerMovementComponent->SprintPressed();
}

void AExtractionGameCharacter::SprintReleased()
{
	if(PlayerMovementComponent == nullptr)
	{
		return;
	}
	
	PlayerMovementComponent->SprintReleased();
}

void AExtractionGameCharacter::CrouchPressed()
{
	PlayerMovementComponent->CrouchPressed();
}

void AExtractionGameCharacter::CrouchReleased()
{
	PlayerMovementComponent->CrouchReleased();
}

void AExtractionGameCharacter::LeftFirePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("StartLfire"));
}

void AExtractionGameCharacter::LeftFireReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("StopLfire"));
}

void AExtractionGameCharacter::RightFirePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("StartRfire"));
}

void AExtractionGameCharacter::RightFireReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("StopRfire"));
}

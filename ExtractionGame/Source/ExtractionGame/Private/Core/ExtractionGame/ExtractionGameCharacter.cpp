#include "Core/ExtractionGame/ExtractionGameCharacter.h"

#include <filesystem>

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "InputActionValue.h"
//#include "Components/InventoryComponent.h"
#include "Components/PlayerHealthComponent.h"
#include "Components/PlayerMovementComponent.h"
//#include "Core/ExtractionGame/TDMPlayerState.h"
#include "Components/GemController.h"
#include "Components/PlayerInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

void AExtractionGameCharacter::ToggleControlLocks(bool x) 
{
	
	//PlayerMovementComponent->SetActive(x);
	bCanMove = x;
	
	//if(x) Controller->EnableInput(GetLocalViewingPlayerController());
	//else Controller->DisableInput(GetLocalViewingPlayerController());
}


void AExtractionGameCharacter::ServerUpdateGaze_Implementation(FVector newGaze)
{
	UE_LOG(LogTemp, Warning, TEXT("Looking in direction: %s"), *newGaze.ToString())
	GazeLocation = newGaze;
}


void AExtractionGameCharacter::ServerUpdateGazeUnreliable_Implementation(FVector newGaze)
{
	GazeLocation = newGaze;
}


void AExtractionGameCharacter::InitializeUIComponents(const AExtractionGameHUD* HUD) const
{
	GemController->Initialize(HUD);
	PlayerHealthComponent->Initialize(HUD);
	
}

AExtractionGameCharacter::AExtractionGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
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

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
	GemController = CreateDefaultSubobject<UGemController>(TEXT("GemController"));

	AbilitySystemComponent = CreateDefaultSubobject<UExtractionAbilitySystemComponent>(TEXT("GAS Ability Controller"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	AttributeSetBase = CreateDefaultSubobject<UExtractionAttributeSet>(TEXT("GAS Attribute Set"));
}


void AExtractionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	OnHealthChanged(1);
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")),true,this);
	TraceParams.bIgnoreTouches = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bIgnoreBlocks = false;
	
	GazeCollisionParams = TraceParams;

	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetCharacterMovement());


	
}

void AExtractionGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsLocallyControlled())
	{
		Server_SetInput(LocalVerticalMovement, LocalHorizontalMovement, LocalVerticalLook, LocalHorizontalLook);
		HandleGaze();

		//Warning constant maybe not needed expense
		ServerUpdateGazeUnreliable(GazeLocation);
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::ResetMove);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Look);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::SprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::SprintReleased);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::CrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::CrouchReleased);

		//When you release it does something...
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::Interact);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::ToggleInventory);
		EnhancedInputComponent->BindAction(SettingsAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::ToggleSettings);
		
		EnhancedInputComponent->BindAction(LeftAttackAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::LeftAttackPressed);
		EnhancedInputComponent->BindAction(RightAttackAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::RightAttackPressed);
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
	DOREPLIFETIME(AExtractionGameCharacter, VerticalLook);
	DOREPLIFETIME(AExtractionGameCharacter, HorizontalLook);
}

void AExtractionGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Warning, TEXT("Possessed by %s, has Authority: %hhd"), *NewController->GetName(), HasAuthority());
	//Server GAS initialization
	/*
	if(AGemPlayerState *state = Cast<AGemPlayerState>(GetPlayerState())) // not nullptr
	{
		AbilitySystemComponent = Cast<UExtractionAbilitySystemComponent>(state->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		state->CreateStateFuncs();
		GemController->SetAbilitySystem(AbilitySystemComponent);
	}
	GemPlayerState = Cast<AGemPlayerState>(NewController->PlayerState);
	GemPlayerState->SetHealth(100.f); */
	//GemController->SmartRecompileGems();
//	InventoryComponent->InitInventory();
	if(IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Possessed by %s, has Authority: %hhd"), *NewController->GetName(), HasAuthority());
		InventoryComponent->InitStartingItems();
	}
}

void AExtractionGameCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Client GAS initialization
	UE_LOG(LogTemp, Warning, TEXT("REP_Player state Enter Has authority: %hhd"), HasAuthority());
	/*
	if(AGemPlayerState *state = Cast<AGemPlayerState>(GetPlayerState())) // not nullptr
	{
		AbilitySystemComponent = Cast<UExtractionAbilitySystemComponent>(state->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		state->CreateStateFuncs();
		GemController->SetAbilitySystem(AbilitySystemComponent);

	} */
	//GemController->SmartRecompileGems();
//	InventoryComponent->InitInventory();
}


void AExtractionGameCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	//SafeBeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Controller Has authority: %hhd"), HasAuthority())
	GemController->SmartRecompileGems();

	if(IsLocallyControlled())
	{
		InventoryComponent->InitStartingItems();
	}
}

void AExtractionGameCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}


void AExtractionGameCharacter::HandleDamage(float amount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, AExtractionGameCharacter* Instigator0, AActor* DamageCauser)
{
	OnDamaged(amount, HitInfo, DamageTags, Instigator0, DamageCauser);
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

bool AExtractionGameCharacter::CanJumpInternal_Implementation() const
{
	PlayerMovementComponent->bWantsToCrouch = false;
	return JumpIsAllowedInternal(); // Ignore restriction for being crouched
}

void AExtractionGameCharacter::SlideJump()
{
	PlayerMovementComponent->SlideJump();
}


void AExtractionGameCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	//bad solution, clients can get rid of issliding check and move while sliding (do we want the player to move while sliding??)
	if (Controller != nullptr && !IsSliding && bCanMove)
	{
		LocalVerticalMovement = MovementVector.Y;
		LocalHorizontalMovement = MovementVector.X;

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AExtractionGameCharacter::Look(const FInputActionValue& Value)
{
	if(!bCanMove) return;
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		LocalVerticalLook = FirstPersonCameraComponent->GetComponentRotation().Pitch;
		LocalHorizontalLook = LookAxisVector.X;

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AExtractionGameCharacter::SprintPressed()
{
	PlayerMovementComponent->SprintPressed();
}

void AExtractionGameCharacter::SprintReleased() 
{
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

void AExtractionGameCharacter::Interact()
{
	if(GazeTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to interact with SOMETHING"))
		GazeTarget->Execute_OnInteract(GazeTargetActor, this);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Tried to interact with NOTHING"))
}

void AExtractionGameCharacter::ToggleInventory()
{
	bInInventory = !bInInventory;

	ToggleControlLocks(!(bInInventory|bInSettings));
	
	if(bInInventory)
	{
		OnInventoryOpened();
	}
	else
	{
		OnInventoryClosed();
		GemController->SmartRecompileGems();
	}
}


void AExtractionGameCharacter::ToggleSettings()
{
	bInSettings = !bInSettings;
	
	ToggleControlLocks(!(bInInventory|bInSettings));
	
	if(bInSettings)
	{
		OnSettingsOpened();
		if(bInInventory)
		{
			OnInventoryClosed();
		}
	}
	else
	{
		OnSettingsClosed();
	}
}

void AExtractionGameCharacter::HandleGaze()
{
	FVector CamLoc;
	FRotator CamRot;

	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
	const FVector StartTrace = CamLoc; // trace start is the camera location
	const FVector Direction = CamRot.Vector();
	const FVector EndTrace = StartTrace + Direction * 100000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks
	
	// Perform trace to retrieve hit info
	

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, GazeCollisionParams); // simple trace function

	
	if(Hit.bBlockingHit)
	{
		constexpr float minDist = 500;
		if(Hit.Distance < minDist)GazeLocation = StartTrace + Direction * minDist;
		else GazeLocation = Hit.ImpactPoint;
		
		if(Hit.Distance <= InteractionDistance)
		{
			//C++ you can't merge these ifs... :/
			if(IInteractable* Usable = Cast<IInteractable>(Hit.GetActor())) // we are looking to a usable object
			{
				if(GazeTarget != Usable)
				{
					ChangeGaze();
				}
				GazeTargetActor = Hit.GetActor();
				GazeTarget = Usable; // as the actor under crosshairs is a usable actor, we store it for the hud.
				GazeTarget->Execute_OnStartFocus(GazeTargetActor);
			}
			else
			{
				ChangeGaze();
			}
		}
		else
		{
			ChangeGaze();
		}
	}
	else
	{
		GazeLocation = Hit.TraceEnd;
		ChangeGaze();
	}

}

void AExtractionGameCharacter::ChangeGaze()
{
	if(!GazeTarget) return;
	GazeTarget->Execute_OnCancelFocus(GazeTargetActor);
	GazeTarget = nullptr;
}

UExtractionAttributeSet* AExtractionGameCharacter::GetAttributeSet() const
{
	return AttributeSetBase;
}

UAbilitySystemComponent* AExtractionGameCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



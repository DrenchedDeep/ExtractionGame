#include "ExtractionGameCharacter.h"

#include <filesystem>

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ExtractionGameInstance.h"
#include "InputActionValue.h"
#include "PlayerHealthComponent.h"
#include "PlayerMovementComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/ExtractionAttributeSet.h"
#include "Components/ExtractionAbilitySystemComponent.h"
#include "Core/AbilityHandlerSubSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

FVector AExtractionGameCharacter::GetPlayerLookPoint()
{
	return LookAtPoint;
}

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

	bAbilitiesInitialized = false;
	AbilitySystemComponent = CreateDefaultSubobject<UExtractionAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UExtractionAttributeSet>(TEXT("Ability Attributes"));
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

	
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")),true,this);
	TraceParams.bIgnoreTouches = true;
	TraceParams.bReturnPhysicalMaterial = true;
	
	GazeCollisionParams = TraceParams;

	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetCharacterMovement());

	UAbilityHandlerSubSystem* MyGameInstance = GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	const TSubclassOf<UGameplayAbility> left = MyGameInstance->GetAbilityByIndex(MyGameInstance->ConvertToIntID(LeftEarth, LeftFire, LeftShadow, LeftWater));
	const TSubclassOf<UGameplayAbility> right = MyGameInstance->GetAbilityByIndex(MyGameInstance->ConvertToIntID(RightEarth, RightFire, RightShadow, RightWater));
	AbilitySystemComponent->SetInputBinding(LeftAttackAction, AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(left, 1, -1, this)));
	AbilitySystemComponent->SetInputBinding(RightAttackAction,AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(right, 1, -1, this)));
		

}

void AExtractionGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsLocallyControlled())
	{
		Server_SetInput(LocalVerticalMovement, LocalHorizontalMovement, LocalVerticalLook, LocalHorizontalLook);
		HandleGaze();
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::ResetMove);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExtractionGameCharacter::Look);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::SprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::SprintReleased);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::CrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::CrouchReleased);

		//When you release it does something...
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AExtractionGameCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::ToggleInventory);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AExtractionGameCharacter::ToggleSettings);
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

void AExtractionGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS initialization
	if(AbilitySystemComponent) // not nullptr
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void AExtractionGameCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Client GAS initialization
	AbilitySystemComponent->InitAbilityActorInfo(this,this);

}

UAbilitySystemComponent* AExtractionGameCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AExtractionGameCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if(GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		//Grants abilities, but only on the server.

		/*Also force binds them to a key... Big no-no for this game.
		 *for (TSubclassOf<UGameplayAbility>& StartUpAbility : Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartUpAbility, 1, static_cast<int32>(StartUpAbility.GetDefaultObject()->AbilityInputID), this
			));
		}*/
		
		//Apply Passive abilities
		for(const TSubclassOf<UGameplayEffect>& GameplayEffect: PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1,  EffectContext);

			if(NewHandle.IsValid())
			{
				//FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
					AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
		bAbilitiesInitialized = true;
	}
}

void AExtractionGameCharacter::HandleDamage(float amount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, AExtractionGameCharacter* Instigator0, AActor* DamageCauser)
{
	OnDamaged(amount, HitInfo, DamageTags, Instigator0, DamageCauser);
}

void AExtractionGameCharacter::HandleHealthChanged(float change, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitialized)
	{
		OnHealthChanged(change, EventTags);
	}
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

void AExtractionGameCharacter::Interact()
{
	if(GazeTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to interact with SOMETHING"))
		GazeTarget->Execute_OnInteract(GazeTargetActor, this);
	}
	UE_LOG(LogTemp, Warning, TEXT("Tried to interact with NOTHING"))
}

void AExtractionGameCharacter::ToggleInventory()
{
	bInInventory = !bInInventory;
	bCanMove = !(bInInventory|bInSettings);
	if(bInInventory)
	{
		OnInventoryOpened();
	}
	else
	{
		OnInventoryClosed();
	}
}


void AExtractionGameCharacter::ToggleSettings()
{
	bInSettings = !bInSettings;
	bCanMove = !(bInInventory|bInSettings);
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
	const FVector EndTrace = StartTrace + Direction * 10000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks
	
	// Perform trace to retrieve hit info
	

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, GazeCollisionParams); // simple trace function

	
	if(Hit.bBlockingHit)
	{
		LookAtPoint = Hit.ImpactPoint;
		UE_LOG(LogTemp, Warning, TEXT("Target is at distance: %f"), Hit.Distance);
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
		LookAtPoint = Hit.TraceEnd;
		ChangeGaze();
	}
}

void AExtractionGameCharacter::ChangeGaze()
{
	if(!GazeTarget) return;
	GazeTarget->Execute_OnCancelFocus(GazeTargetActor);
	GazeTarget = nullptr;
}

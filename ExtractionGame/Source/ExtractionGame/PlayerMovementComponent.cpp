#include "PlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"


UPlayerMovementComponent::UPlayerMovementComponent()
{
}



//checks two moves, the current move and new move and checks if we can combine them to save bandwidth
bool UPlayerMovementComponent::FSavedMove_Player::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
	float MaxDelta) const
{
	const FSavedMove_Player* NewPlayerMove = static_cast<FSavedMove_Player*>(NewMove.Get());

	if(Saved_bWantsToSprint != NewPlayerMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

//reset a saved moved object to be empty
void UPlayerMovementComponent::FSavedMove_Player::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

//only send the important data every tick
uint8 UPlayerMovementComponent::FSavedMove_Player::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if(Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	
	return Result;
}

void UPlayerMovementComponent::FSavedMove_Player::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UPlayerMovementComponent* PlayerMovement = Cast<UPlayerMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = PlayerMovement->bWantsToSprint;
}

//take data from saved move and apply it
void UPlayerMovementComponent::FSavedMove_Player::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UPlayerMovementComponent* PlayerMovement = Cast<UPlayerMovementComponent>(C->GetCharacterMovement());

	PlayerMovement->bWantsToSprint = Saved_bWantsToSprint;
}

UPlayerMovementComponent::FNetworkPredictionData_Client_Player::FNetworkPredictionData_Client_Player(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UPlayerMovementComponent::FNetworkPredictionData_Client_Player::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Player());
}

FNetworkPredictionData_Client* UPlayerMovementComponent::GetPredictionData_Client() const
{
	if(ClientPredictionData == nullptr)
	{
		UPlayerMovementComponent* MovementComp = const_cast<UPlayerMovementComponent*>(this);

		MovementComp->ClientPredictionData = new FNetworkPredictionData_Client_Player(*this);
		MovementComp->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MovementComp->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

//set state based on flags
void UPlayerMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UPlayerMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	if(MovementMode == MOVE_Walking)
	{
		if(bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
	
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UPlayerMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
}

void UPlayerMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	//proxies will receive state from replicated vars
	if(CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		if(!Character->IsSliding)
		{
			//dont want to use local delta seconds because tick rate between clients/server will be different
			Character->SlideTimer += GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		}
		
		if (IsPlayerMovementMode(PMOVE_Slide) && !bWantsToCrouch)
		{
			ExitSlide();
		}
		
		if(MovementMode == MOVE_Walking && bWantsToCrouch && Character->GetVelocity().Size() >= 500 && CanSlideInCurrentState())
		{
			EnterSlide();
		}

		//bad
		Character->IsSprinting = MaxWalkSpeed == Sprint_MaxWalkSpeed && !Character->IsSliding;
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UPlayerMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	if(CharacterOwner->bIsCrouched && Character->IsSprinting && !Character->IsSliding)
	{
		bWantsToCrouch = false;
		UnCrouch();
	}
		
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

void UPlayerMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if(CustomMovementMode == PMOVE_Slide)
	{
		PhysSlide(deltaTime, Iterations);
	}
}

bool UPlayerMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsPlayerMovementMode(PMOVE_Slide);
}

bool UPlayerMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

void UPlayerMovementComponent::SprintPressed()
{
	//get rid of local check if we keep developing 
	if(Character->IsSliding)
	{
		bWantsToSprint = false;
		return;
	}
	
	bWantsToSprint = true;
}

void UPlayerMovementComponent::SprintReleased()
{
	bWantsToSprint = false;
}

void UPlayerMovementComponent::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;
}

void UPlayerMovementComponent::CrouchReleased()
{
	bWantsToCrouch = bToggleCrouch && bWantsToCrouch;
}

bool UPlayerMovementComponent::IsPlayerMovementMode(EPlayerMovementMode MoveMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == MoveMode;
}

void UPlayerMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	Character->IsSliding = true;
	Character->OnSlideStart();

	FHitResult SurfaceHit;
	GetSlideSurface(SurfaceHit);
	
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	
	Velocity += VelPlaneDir + Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	SetMovementMode(MOVE_Custom, PMOVE_Slide);
}

void UPlayerMovementComponent::ExitSlide()
{
	Character->IsSliding = false;
	Character->SlideTimer = 0.0f;
	Character->OnSlideEnd();
	const FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}

void UPlayerMovementComponent::PhysSlide(float DeltaTime, int32 Iterations)
{
	if(DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	GetSlideSurface(SurfaceHit);
	Velocity += Slide_GravityForce * FVector::DownVector * DeltaTime;

	CalcVelocity(DeltaTime, Slide_Friction, true, GetMaxBrakingDeceleration());

	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();

	FHitResult Hit(1.f);
	GetSlideSurface(SurfaceHit);
	FVector Adjusted = Velocity * DeltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();

	//slide the character
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if(Hit.Time < 1.f)
	{
		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	if(Velocity.SizeSquared() < pow(Slide_MinSpeed, 2))
	{
		ExitSlide();
	}

	if(!bJustTeleported)
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}
}

bool UPlayerMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector End = Start + Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	const FName ProfileName = TEXT("BlockAll");

	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, Character->GetIgnoreCharacterParams());
}

bool UPlayerMovementComponent::CanSlideInCurrentState() const
{
	return !IsFalling() && IsMovingOnGround() && !Character->IsSliding && Character->SlideTimer >= Slide_Timer && !Character->bIsCrouched;
}


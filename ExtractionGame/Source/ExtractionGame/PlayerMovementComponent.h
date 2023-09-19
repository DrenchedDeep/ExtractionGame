#pragma once

#include "CoreMinimal.h"
#include "ExtractionGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"


UENUM(BlueprintType)
enum EPlayerMovementMode
{
	PMOVE_None    UMETA(Hidden),
	PMOVE_Slide   UMETA(DisplayName = "Slide"),
	PMOVE_Max     UMETA(Hidden),
};


UCLASS()
class EXTRACTIONGAME_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	//snapshot of all state in movement comp required to produce a move in a single frame
	class FSavedMove_Player : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		//Flag
		uint8 Saved_bWantsToSprint:1;

		uint8 Saved_bPrevWantsToCrouch:1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	//telling core component to use our SavedMove data
	 class FNetworkPredictionData_Client_Player : public FNetworkPredictionData_Client_Character
	{
	 public:
		FNetworkPredictionData_Client_Player(const UCharacterMovementComponent& ClientMovement);
		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};


	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 350;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 500;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3;


	UPROPERTY(Transient) AExtractionGameCharacter* Character;

	bool bWantsToSprint;

public:
	UPlayerMovementComponent();
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void InitializeComponent() override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	
public:
	void SprintPressed();
	void SprintReleased();

	void CrouchPressed();

	UFUNCTION(BlueprintPure)
	bool IsPlayerMovementMode(EPlayerMovementMode MoveMode) const;

private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

	bool CanSlideInCurrentState();
};

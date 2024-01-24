#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
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


	FDelegateHandle OnSpeedChangeHandle;
	void ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const;
	FActiveGameplayEffectHandle SpeedHandle;


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
	float currentSpeed;
	float currentCrouchSpeed;

	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 350;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 500;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3;
	UPROPERTY(EditDefaultsOnly) float Slide_Timer = .5f;
	UPROPERTY(EditDefaultsOnly) float LeapStrength = 1500;
	UPROPERTY(EditDefaultsOnly) float LeapAngle = 30;




	UPROPERTY(Transient) AExtractionGameCharacter* Character;

	bool bWantsToSprint;

	virtual void BeginPlay() override;


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> BaseSpeedEffect;
	
public:


	
	UPlayerMovementComponent();
	
	UPROPERTY(EditDefaultsOnly) bool bToggleCrouch;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	void SlideJump();

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	//virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void InitializeComponent() override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	void OnSpeedChanged(const FOnAttributeChangeData& Data);

public:
	void SprintPressed();
	void SprintReleased();

	void CrouchPressed();
	void CrouchReleased();

	UFUNCTION(BlueprintPure)
	bool IsPlayerMovementMode(EPlayerMovementMode MoveMode) const;


	
private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
	void Leap();
	bool CanSlideInCurrentState() const;
};

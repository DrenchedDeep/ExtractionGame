// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ExtractionGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AExtractionGameCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	bool bSprintPressed;
	bool bCrouchPressed;

	float LocalVerticalMovement;
	float LocalHorizontalMovement;
	
public:
	UPROPERTY(EditAnywhere, Category=Movement)
	float WalkMovementSpeed;
	UPROPERTY(EditAnywhere, Category=Movement)
	float SprintMovementSpeed;
	UPROPERTY(EditAnywhere, Category=Movement)
	float CrouchMovementSpeed;

	UPROPERTY(EditAnywhere, Category=Crouching)
	FVector DefaultCameraLocation = FVector(-10.f, 0.f, 60.f);
	UPROPERTY(EditAnywhere, Category=Crouching)
	FVector CrouchCameraLocation;
	UPROPERTY(EditAnywhere, Category=Crouching)
	float CrouchCameraOffsetSmoothing = 0.5f;

	
public:
	AExtractionGameCharacter();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsSprinting;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsCrouching;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsSliding;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float VerticalMovement;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float HorizontalMovement;
	
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void StartSprintPressed();
	void StopSprintPressed();
	void CrouchPressed();

	UFUNCTION(Server, Unreliable)
	void Server_SendInput(float LocVerticalMovement, float LocHorizontalMovement, bool LocSprintPressed, bool LocCrouchPressed);

	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	float GetMovementSpeed() const;
};


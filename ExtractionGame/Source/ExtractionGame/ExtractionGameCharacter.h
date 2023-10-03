// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GemController.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ExtractionGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class UGameplayAbility;
class UExtractionAbilitySystemComponent;
class UExtractionAttributeSet;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AExtractionGameCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	/*------------------- Input actions -----------------------*/
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LeftAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* RightAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HeadAbilityAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	void Move(const FInputActionValue& Value);
	void ResetMove();
	void Look(const FInputActionValue& Value);

	void SprintPressed();
	void SprintReleased();

	void CrouchPressed();
	void CrouchReleased();

	void Interact();
	
	
	/*--------------------------------------------------------*/
	float LocalVerticalMovement;
	float LocalHorizontalMovement;
	float LocalVerticalLook;
	float LocalHorizontalLook;

	UPROPERTY(EditAnywhere)
	UGemController *GemController;


public:
	AExtractionGameCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated, BlueprintReadOnly)
	float VerticalMovement;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float HorizontalMovement;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float VerticalLook;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float HorizontalLook;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsSliding;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float SlideTimer;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsSprinting;
	

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
	class UPlayerMovementComponent* PlayerMovementComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Health)
	class UPlayerHealthComponent* PlayerHealthComponent;
	

public:

	

	// -- Why are any of these public -- //
	
	//useful for sfx/cam shake/etc
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlideStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlideEnd();

	UFUNCTION(Server, Unreliable)
	void Server_SetInput(float VerticalMove, float HorizontalMove, float VertLook, float HorLook);
	
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/***************************************
	 * Ability System
	 ****************************************/
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void AddStartupGameplayAbilities();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float amount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AExtractionGameCharacter* Instigator0, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float change, const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float amount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AExtractionGameCharacter* Instigator, AActor* DamageCauser);

	virtual void HandleHealthChanged(float change, const struct FGameplayTagContainer& EventTags);

	friend UExtractionAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int LeftEarth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int LeftFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int LeftWater;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int LeftShadow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int RightEarth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int RightFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int RightWater;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TEMP|Abilities")
	int RightShadow;
	
	UPROPERTY()
	uint8 bAbilitiesInitialized:1;


public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	FCollisionQueryParams GetIgnoreCharacterParams() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UExtractionAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UExtractionAttributeSet> AttributeSet;

};

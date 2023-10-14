// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Core/Interactable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ExtractionGameCharacter.generated.h"

class UInventoryComponent;
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* LeftAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* RightAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* HeadAbilityAction;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Inventory)
	UInventoryComponent* InventoryComponent;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SettingsAction;

	void Move(const FInputActionValue& Value);
	void ResetMove();
	void Look(const FInputActionValue& Value);

	void SprintPressed();
	void SprintReleased();
	
	void CrouchPressed();
	void CrouchReleased();

	void Interact();

	void ToggleInventory();
	void ToggleSettings();

	void HandleGaze();
	void ChangeGaze();
	
	bool bInInventory;
	bool bInSettings;
	bool bCanMove = true;
	
	FCollisionQueryParams GazeCollisionParams;
	
	IInteractable* GazeTarget;
	UPROPERTY()
	AActor* GazeTargetActor;

	
	
	/*--------------------------------------------------------*/
	float LocalVerticalMovement;
	float LocalHorizontalMovement;
	float LocalVerticalLook;
	float LocalHorizontalLook;
	
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float InteractionDistance = 20;

	void ToggleControlLocks(bool x);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInventoryOpened();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInventoryClosed();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSettingsOpened();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSettingsClosed();
	
	UFUNCTION(BlueprintImplementableEvent)
	void LeftAttackPressed();

	UPROPERTY(BlueprintReadOnly)
	FVector GazeLocation;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerUpdateGaze(FVector newGaze);
	
	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void ServerUpdateGazeUnreliable(FVector newGaze);
	
	UFUNCTION(BlueprintImplementableEvent)
	void RightAttackPressed();
	
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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
	class UPlayerMovementComponent* PlayerMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Health)
	class UPlayerHealthComponent* PlayerHealthComponent;
	

public:

	FORCEINLINE bool CanMove() const { return bCanMove;}

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

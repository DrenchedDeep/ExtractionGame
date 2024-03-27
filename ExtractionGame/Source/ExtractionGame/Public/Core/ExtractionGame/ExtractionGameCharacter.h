// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Abilities/ExtractionAttributeSet.h"
#include "Core/ExtractionGame/Interactable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "Core/CharacterPawnBase.h"
#include "ExtractionGameCharacter.generated.h"

class UHamiltonController;
class UPlayerInventoryComponent;
class UGemController;
class UExtractionAbilitySystemComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;



DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AExtractionGameCharacter : public ACharacterPawnBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UHamiltonController* HamiltonController;
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
	UPlayerInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Inventory)
	UGemController* GemController;

	
private:

	UPROPERTY(EditAnywhere)
	UExtractionAttributeSet* AttributeSetBase;

	UPROPERTY(EditAnywhere)
	UExtractionAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SettingsAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HamiltonAction;
	
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

	void HamiltonPressed();
	void HamiltonReleased();

	void HamiltonStartPressed();
	void HamiltonStartReleased();

	void HandleGaze();
	void ChangeGaze();
	
	bool bInSettings;
	bool bCanMove = true;
	
	FCollisionQueryParams GazeCollisionParams;
	
	IInteractable* GazeTarget;
	UPROPERTY()
	AActor* GazeTargetActor;



	//Could not be bothered to move these. Sorry.
	bool bWantsToLeftFire;
	bool bWantsToRightFire;

public:
	//Needs to be here cus circular depend
	bool bIsLeftAutomatic;
	bool bIsRightAutomatic;
	
private:
	
	void StartFireLeft() {if(bIsLeftAutomatic)bWantsToLeftFire = true; else LeftAttackPressed(); }
	void StartFireRight() {if(bIsRightAutomatic)bWantsToRightFire = true; else RightAttackPressed(); }
	void StopFireLeft() {bWantsToLeftFire = false;}
	void StopFireRight() { bWantsToRightFire = false; }
	
	
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

	UFUNCTION(BlueprintImplementableEvent)
	void HamiltonProcessStarted();

	void HamiltonFinished();
	
	UFUNCTION(BlueprintImplementableEvent) void HamiltonProcessStopped(); 
	

	UFUNCTION(BlueprintImplementableEvent)
	void HamiltonStartedEverybody(bool Cancelled);
	
	void InitializeUIComponents(const class AExtractionGameHUD* HUD) const;

	UFUNCTION(BlueprintImplementableEvent)
	void InitGemUI();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float healthPercent);
	
	UPROPERTY(BlueprintReadOnly)
	FVector GazeLocation;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUpdateGaze(FVector newGaze);
	
	UFUNCTION(Server, Unreliable, BlueprintCallable)
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
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool IsSliding;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float SlideTimer;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsSprinting;

	UPROPERTY(BlueprintReadOnly)
	bool bInInventory;

	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
	class UPlayerMovementComponent* PlayerMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Health)
	class UPlayerHealthComponent* PlayerHealthComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStateUpdated();

	

public:

	

	//UPROPERTY(Replicated) // It's per object
	
	//static float KnockBackMultiplier = 1;
	
	FORCEINLINE bool CanMove() const { return bCanMove;}

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlideStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlideEnd();
	
	UFUNCTION(Server, Unreliable)
	void Server_SetInput(float VerticalMove, float HorizontalMove, float VertLook, float HorLook);

	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathServer();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathClient();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttacked(FVector FromLocation);

	
	
	
protected:



	
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;
	virtual void BeginDestroy() override;

	//event for when is safe to access things like controller, playerstate, etc


	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float amount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AExtractionGameCharacter* Instigator0, AActor* DamageCauser);

	virtual void HandleDamage(float amount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AExtractionGameCharacter* Instigator, AActor* DamageCauser);

	
	friend UExtractionAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;



	
	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

	
	


public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	FCollisionQueryParams GetIgnoreCharacterParams() const;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	//TObjectPtr<UExtractionAbilitySystemComponent> AbilitySystemComponent;

//	UPROPERTY()
//	UExtractionAttributeSet AttributeSet;

	virtual bool CanJumpInternal_Implementation() const override;

	UFUNCTION(BlueprintCallable)
	void SlideJump();

	//GAS HEALTH LATER:
	
	
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UExtractionAttributeSet* GetAttributeSet() const;

public:
	UFUNCTION(BlueprintCallable)
	void AddEssence(float Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveEssence(float Amount);
	
	UFUNCTION(BlueprintCallable)
	float GetEssence() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEssenceUpdated();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tutorial") float TutorialEssenceThreshold;
	
private:
	UFUNCTION()
	void OnRep_EssenceUpdate();
	
	UPROPERTY(ReplicatedUsing=OnRep_EssenceUpdate)
	int32 EssenceUpdate;

};

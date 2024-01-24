

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/NetworkPredictionInterface.h"

#include "SpaceShip.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
UCLASS()
class EXTRACTIONGAME_API ASpaceShip : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	FVector2D movementDirection;
	
	FVector rotator;
	
	//Store niagara particle Systems
	void OnLook(const FInputActionValue& Value);
	void OnDirectionChanged(const FInputActionValue& Value);
	void OnDirectionStopped();
	void StartBoost();
	void StopBoost();

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* Move;

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* Look;
	
	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* Thrust;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* ShipMappingContext;

	float curRot;

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

		

protected:

	
	UPROPERTY(BlueprintReadOnly)
	bool isCrashed;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Emitters")
	UNiagaraComponent* Primary; 
	
	UPROPERTY(BlueprintReadWrite, Category = "Emitters|FRONT")
	UNiagaraComponent* FrontLeft;
	UPROPERTY(BlueprintReadWrite, Category = "Emitters|FRONT")
	UNiagaraComponent* FrontMid;
	UPROPERTY(BlueprintReadWrite, Category = "Emitters|FRONT")
	UNiagaraComponent* FrontRight;

	UPROPERTY(BlueprintReadWrite, Category = "Emitters|BACK")
	UNiagaraComponent* BackLeft;
	UPROPERTY(BlueprintReadWrite, Category = "Emitters|BACK")
	UNiagaraComponent* BackMid;
	UPROPERTY(BlueprintReadWrite, Category = "Emitters|BACK")
	UNiagaraComponent* BackRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageEvent DamageType;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCrush(FHitResult hit);
	
	
	
	
	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float regularSpeed;

	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float boostSpeed;

	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float rotationSpeed;

	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float camPanSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float MaxPitch;


	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float currentSpeed;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetMovementDirection(FVector2D NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSpeed(float NewValue);
	
	//Replicate the speed?
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CrashLand(FHitResult HitResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ClientCrashLandEffects(FHitResult HitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnThrustBegin();

	UFUNCTION(BlueprintImplementableEvent)
	void OnThrustEnd();
	
	UFUNCTION(NetMulticast, Reliable)
	void ClientCrashLand(FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void ClientOnCrush(FHitResult HitResult);
	
	
	UFUNCTION(BlueprintCallable)
	void MoveToWorldSpawn();
	
	//virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void OnDiscardShip();

	virtual void PostInitializeComponents() override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};

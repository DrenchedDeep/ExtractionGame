

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"

#include "SpaceShip.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
UCLASS()
class EXTRACTIONGAME_API ASpaceShip : public APawn
{
	GENERATED_BODY()

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
	bool isCrashed;
	
public:
	// Sets default values for this pawn's properties
	ASpaceShip();

		

protected:
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


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float currentSpeed;

	//Replicate the speed?
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CrashLand(FHitResult HitResult);
	//virtual void PossessedBy(AController* NewController) override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};

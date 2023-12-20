

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"

#include "SpaceShip.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Move;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Look;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Thrust;

	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float regularSpeed;

	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float boostSpeed;

	UPROPERTY(EditDefaultsOnly, Category=Stats)
	float rotationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float currentSpeed;

	
	//UFUNCTION(BlueprintImplementableEvent)
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};

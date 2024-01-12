

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/Interactable.h"
#include "Gem.generated.h"

/**
 * 
 */



UENUM(BlueprintType)
enum EGemType : uint8 {
	Earth        UMETA(DisplayName="Earth|Green"),
	Fire     UMETA(DisplayName="Fire|Red"),
	Shadow    UMETA(DisplayName="Shadow|Purple"),
	Water       UMETA(DisplayName="Water|Blue")
};

UCLASS(Blueprintable)
class AGem : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Replicated)
	float Polish;

	UPROPERTY(EditAnywhere, Replicated)
	TEnumAsByte<EGemType> GemType;

public:
	AGem();
	
	UFUNCTION(BlueprintCallable)
	float GetPolish () const;
	
	UFUNCTION(BlueprintCallable)
	EGemType GetGemType () const;

	void SetPolish(float polish) { Polish = polish; }
	void SetGemType(EGemType gemType) { GemType = gemType; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	
};

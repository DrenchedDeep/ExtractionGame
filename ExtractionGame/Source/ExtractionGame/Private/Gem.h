

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Core/Interactable.h"
#include "Gem.generated.h"

/**
 * 
 */



UENUM(BlueprintType)
enum class EGemType : uint8 {
	Earth        UMETA(DisplayName="Earth|Green"),
	Fire     UMETA(DisplayName="Fire|Red"),
	Shadow    UMETA(DisplayName="Shadow|Purple"),
	Water       UMETA(DisplayName="Water|Blue")
};

UCLASS(Blueprintable)
class AGem : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Polish;

	UPROPERTY(EditAnywhere)
	EGemType GemType;
	
	public:

	UFUNCTION(BlueprintCallable)
	float GetPolish () const;
	
	UFUNCTION(BlueprintCallable)
	EGemType GetGemType () const;
	
	
	
	
	
	
};



#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Gem.generated.h"

/**
 * 
 */



UENUM(BlueprintType)
enum class EGemType : uint8 {
	Fire     UMETA(DisplayName="Red"),
	Water       UMETA(DisplayName="Blue"),
	Light        UMETA(DisplayName="Yellow"),
	Dark    UMETA(DisplayName="Green"),
};

UCLASS()
class UGem : public UStaticMeshComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Polish;

	UPROPERTY(EditAnywhere)
	EGemType GemType;
	
	public:
	
	float GetPolish () const;
	EGemType GetGemType () const;
	
	
	
	
	
	
};

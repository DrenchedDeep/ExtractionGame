

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Gem.generated.h"

/**
 * 
 */



UENUM(BlueprintType)
enum class EGemType : uint8 {
	None = 0      UMETA(DisplayName="None"),
	Red = 1      UMETA(DisplayName="Red"),
	Blue = 2       UMETA(DisplayName="Blue"),
	Yellow = 4       UMETA(DisplayName="Yellow"),
	Green  = 8    UMETA(DisplayName="Green"),
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

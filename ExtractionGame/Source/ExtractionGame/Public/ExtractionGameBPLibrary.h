

#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComp.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExtractionGameBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UExtractionGameBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION( BlueprintPure, meta=( WorldContext = "WorldContextObject", CallableWithoutWorldContext), Category = "HUD|Util" )
	static void FindScreenEdgeLocationForWorldLocation( UObject* WorldContextObject, const FVector& InLocation,
	const float EdgePercent, const FVector2D ViewportCenterLoc, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen );
	
	UFUNCTION(BlueprintPure)
	static ERarityType GetRarityTypeFromGemPurity(float Purity);

	UFUNCTION(BlueprintPure)
	static int32 GetAdjustedPrice(ERarityType Rarity, int32 BasePrice, float Purity);
};


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapInfo.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UMapInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	protected:
	
	UPROPERTY(EditDefaultsOnly)
	float innerRing;
	UPROPERTY(EditDefaultsOnly)
	float outerRing;
	UPROPERTY(EditDefaultsOnly)
	float minHeight;
	UPROPERTY(EditDefaultsOnly)
	float maxHeight;
	UPROPERTY(EditDefaultsOnly)
	float additionalUpOffset;

	public:
	float InnerRing() const { return innerRing; }
	float OuterRing() const { return outerRing; }
	float MinHeight() const { return minHeight; }
	float MaxHeight() const { return maxHeight; }
	float AdditionalUpOffset() const { return additionalUpOffset; }
	
};

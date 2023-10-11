

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="ItemStats")
	FName ItemName;
	UPROPERTY(EditDefaultsOnly,  Category="ItemStats")
	UTexture2D* ItemIcon;
	UPROPERTY(EditDefaultsOnly,  Category="ItemStats")
	int ItemCost;
};

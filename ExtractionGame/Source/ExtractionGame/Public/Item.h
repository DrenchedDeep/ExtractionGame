

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ItemStats")
	FName ItemName;
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category="ItemStats")
	UTexture2D* ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="ItemStats")
	int ItemCost;
};



#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/ExtractionGame/Gem.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum EItemType : uint8
{
	Misc = 0    UMETA(DisplayName = "Misc")
//	Gem = 1    UMETA(DisplayName = "Gem"),
};

UCLASS()
class EXTRACTIONGAME_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ItemStats")
	TEnumAsByte<EItemType> ItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ItemStats")
	uint8 ItemID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ItemStats")
	FName ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ItemStats")
	FString ItemDescription;
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category="ItemStats")
	UTexture2D* ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="ItemStats")
	int ItemCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="ItemStats")
	bool bCanStack = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="ItemStats")
	int MaxStack = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="GemStats")
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="GemStats")
	float DefaultPolish;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="GemStats")
	FColor GemColor;
};

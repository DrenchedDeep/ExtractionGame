

#pragma once

#include "CoreMinimal.h"
#include "Components/StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "ShopManager.generated.h"

USTRUCT(BlueprintType)
struct FShopItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Count;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bOverridePolish;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float PolishAmount = 25;
};

UCLASS()
class EXTRACTIONGAME_API AShopManager : public AActor
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FShopItem> StartingItems;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* ShopInventory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* BuyInventory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* SellInventory;

protected:
	AShopManager();
};



#pragma once

#include "CoreMinimal.h"
#include "Components/StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "ShopManager.generated.h"

UCLASS()
class EXTRACTIONGAME_API AShopManager : public AActor
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> StartingItems;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* ShopInventory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* BuyInventory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* SellInventory;

protected:
	AShopManager();
};

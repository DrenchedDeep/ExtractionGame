

#pragma once

#include "CoreMinimal.h"
#include "Components/StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "ShopManager.generated.h"

UCLASS()
class EXTRACTIONGAME_API AShopManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> StartingItems;
	
public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* ShopInventory;

protected:
	AShopManager();
};

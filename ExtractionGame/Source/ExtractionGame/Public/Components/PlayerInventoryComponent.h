

#pragma once

#include "CoreMinimal.h"
#include "InventoryComp.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UPlayerInventoryComponent : public UInventoryComp
{
	GENERATED_BODY()



public:
	void InitStartingItems();
};



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



protected:
	virtual void BeginPlay() override;
};

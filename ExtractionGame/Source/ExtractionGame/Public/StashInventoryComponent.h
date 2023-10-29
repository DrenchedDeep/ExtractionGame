

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "StashInventoryWidget.h"
#include "StashInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UStashInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	void Init(UStashInventoryWidget* InvenWidget, int32 InventorySize, bool bIsStash);
	
	virtual void AddItem(UItem* Item, int StackSize, bool bClientSimulation, int SlotID = -1) override;
	virtual void RemoveItem(int InventoryID, int StackSize) override;
	virtual void TransferSlots(USlotWidget* OldSlot, USlotWidget* NewSlot) override;
	
	virtual FInventoryItem GetInventoryItem(int InventoryID) override;
	TArray<FInventoryItem> StashItems;
};

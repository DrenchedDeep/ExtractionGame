#include "StashInventoryComponent.h"
#include "MainMenuHUD.h"


void UStashInventoryComponent::Init(UStashInventoryWidget* InvenWidget, int32 InventorySize, bool bIsStash)
{
	if(!bIsStash)
	{
		InventoryWidget = InvenWidget->StashGridPanel;
		InventoryWidget->Init(this, InventorySize, false);
	}
	else
	{
		InventoryWidget = InvenWidget->PlayerInventoryGridPanel;
		InventoryWidget->Init(this, InventorySize);

	}

	for(int i = 0; i < StartingItems.Num(); i++)
	{
		AddItem(StartingItems[i].ItemID, StartingItems[i].Stack, false);
	}
}

void UStashInventoryComponent::AddItem(UItem* Item, int StackSize, bool bClientSimulation, int SlotID)
{
	const int InventoryIndex = FMath::RandRange(0, 100000);
	
	USlotWidget* SlotWidget;

	if(SlotID == -1)
	{
		SlotWidget = InventoryWidget->GetNextSlot();
	}
	else
	{
		SlotWidget = InventoryWidget->GetSlot(SlotID);
	}
	
	if(SlotWidget)
	{
		FInventoryItem InventoryItem(InventoryIndex, SlotWidget->GetSlotID(), StackSize, Item);

		SlotWidget->PredictVisuals(Item, StackSize);
		SlotWidget->ReconcileVisuals(InventoryItem);
		SlotWidget->SetInventoryIndex(InventoryIndex);

		StashItems.Add(InventoryItem);
	}
}

void UStashInventoryComponent::RemoveItem(int InventoryID, int StackSize)
{
	UE_LOG(	LogTemp, Warning, TEXT("Removing Item"))
	for(int i = 0; i < StashItems.Num(); i++)
	{
		const int Index = StashItems[i].InventoryID;
		if(InventoryID == Index)
		{
			UE_LOG(	LogTemp, Warning, TEXT("found it"))
			InventoryWidget->GetSlot(StashItems[i].SlotID)->Reset();
			StashItems.RemoveAt(i);
			break;
		}
	}
}

void UStashInventoryComponent::TransferSlots(USlotWidget* OldSlot, USlotWidget* NewSlot)
{
	//if the new slot already has an item, we want to swap the slots items
	if(NewSlot->bIsOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("New Slot already has item"));
		return;
	}
	for(int i = 0; i < StashItems.Num(); i++)
	{
		if(OldSlot->GetInventoryIndex() == StashItems[i].InventoryID)
		{
			StashItems[i].SlotID = NewSlot->GetSlotID();
			break;
		}
	}
	
	
	//update visuals
	NewSlot->PredictVisuals(OldSlot->GetCurrentItem(),  GetInventoryItem(OldSlot->GetInventoryIndex()).StackSize);
	NewSlot->SetInventoryIndex(OldSlot->GetInventoryIndex());
	OldSlot->Reset();
	
}

FInventoryItem UStashInventoryComponent::GetInventoryItem(int InventoryID)
{
	FInventoryItem InventoryItem = {};

	for(int i = 0; i < StashItems.Num(); i++)
	{
		if(InventoryID == StashItems[i].InventoryID)
		{
			InventoryItem = StashItems[i];
		}
	}

	return InventoryItem;
}


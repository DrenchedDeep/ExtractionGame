


#include "UI/Widgets/SlotWidget.h"

#include "UI/Widgets/InventoryWidget.h"
#include "Core/ExtractionGame/Item.h"
#include "Components/TextBlock.h"


void USlotWidget::Init(UInventoryComponent* InventoryComponent, int ID)
{
	if(!SlotIconImage)
	{
		return;
	}
	
	SlotID = ID;
	Inventory = InventoryComponent;
	SlotIconImage->SetVisibility(ESlateVisibility::Hidden); 
	SlotStackText->SetVisibility(ESlateVisibility::Hidden);
}

void USlotWidget::Reset()
{
	//print("Resetting Slot");
	CurrentItem = nullptr;
	SlotIconImage->SetVisibility(ESlateVisibility::Hidden); 
	SlotStackText->SetVisibility(ESlateVisibility::Hidden);
	InventoryIndex = -1;
	bIsOccupied = false;
}

void USlotWidget::TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID)
{
	if(USlotWidget* OldSlot = SourceInventoryComponent->InventoryWidget->GetSlots()[TargetSlotID])
	{
		if(Inventory != SourceInventoryComponent)
		{
			Inventory->AddItem(OldSlot->GetCurrentItem(), OldSlot->GetCurrentStack(), false, SlotID);

			SourceInventoryComponent->RemoveItem(OldSlot->GetInventoryIndex(),
				SourceInventoryComponent->GetInventoryItem(OldSlot->GetInventoryIndex()).StackSize);
			return;
		}
		
		if(OldSlot == this)
		{
			return;
		}

		if(!OldSlot->GetCurrentItem())
		{
			return;
		}
		
		Inventory->TransferSlots(OldSlot, this);
	}
}

void USlotWidget::PredictVisuals(UItem* Item, int Stack)
{

	if(Item == nullptr)
	{
		return;
	}

	CurrentItem = Item;
	SlotIconImage->SetVisibility(ESlateVisibility::Visible);
	SlotIconImage->SetBrushFromTexture(Item->ItemIcon);
	
	if(Item->bCanStack)
	{
		SlotStackText->SetText(FText::FromString(FString::FromInt(Stack)));
		SlotStackText->SetVisibility(ESlateVisibility::Visible);
	}
	bIsOccupied = true;
}

void USlotWidget::ReconcileVisuals(FInventoryItem& Item)
{
	if(Item.ItemID == nullptr)
	{
		return;
	}
	
	if(CurrentItem == Item.ItemID)
	{
		return;
	}

	CurrentItem = Item.ItemID;
	InventoryIndex = Item.InventoryID;
	SlotIconImage->SetVisibility(ESlateVisibility::Visible);
	SlotIconImage->SetBrushFromTexture(CurrentItem->ItemIcon);
	
	if(Item.ItemID->bCanStack)
	{
		SlotStackText->SetText(FText::FromString(FString::FromInt(Item.StackSize)));
		SlotStackText->SetVisibility(ESlateVisibility::Visible);
	}
	bIsOccupied = true;
}

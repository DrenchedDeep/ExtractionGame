


#include "SlotWidget.h"

#include "InventoryWidget.h"
#include "Item.h"
#include "Components/TextBlock.h"


void USlotWidget::Init(UInventoryComponent* InventoryComponent, int ID)
{
	SlotID = ID;
	Inventory = InventoryComponent;
	SlotIconImage->SetVisibility(ESlateVisibility::Hidden); 
	SlotStackText->SetVisibility(ESlateVisibility::Hidden);
}

void USlotWidget::Reset()
{
	CurrentItem = nullptr;
	SlotIconImage->SetVisibility(ESlateVisibility::Hidden); 
	SlotStackText->SetVisibility(ESlateVisibility::Hidden);
	InventoryIndex = -1;
	bIsOccupied = false;
}

/*/

void USlotWidget::InitItem(UInventoryComponent* InventoryComponent, int InvenIndex)
{
	InventoryIndex = InvenIndex;

	Inventory = InventoryComponent;
	bIsOccupied = true;

	SlotIconImage->SetVisibility(ESlateVisibility::Visible);
	SlotIconImage->SetBrushFromTexture(Inventory->InventoryItems[InventoryIndex].ItemID->ItemIcon);
	SlotStackText->SetText(FText::FromString(FString::FromInt(Inventory->InventoryItems[InventoryIndex].StackSize)));
	SlotStackText->SetVisibility(ESlateVisibility::Visible);
}
/*/

void USlotWidget::TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID)
{
	if(SourceInventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("inventory invalid"));
	}
	
	//for now we dont want to transfer to different inventories
	if(Inventory != SourceInventoryComponent)
	{
		return;
	}
	
	if(USlotWidget* OldSlot = SourceInventoryComponent->InventoryWidget->GetSlots()[TargetSlotID])
	{
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

void USlotWidget::ReconcileVisuals(const FInventoryItem& Item)
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

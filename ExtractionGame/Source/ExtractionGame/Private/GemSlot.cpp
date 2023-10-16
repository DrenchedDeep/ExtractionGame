


#include "GemSlot.h"

#include "InventoryWidget.h"

void UGemSlot::TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID)
{
	//check if item is a gem and if it is, transfer it to the gem slot

	if(const USlotWidget* OldSlot = SourceInventoryComponent->InventoryWidget->GetSlots()[TargetSlotID])
	{
		if(OldSlot->GetCurrentItem()->ItemType == EItemType::Gem)
		{
			Super::TransferSlots(SourceInventoryComponent, TargetSlotID);
		}
	} 
}

void UGemSlot::PredictVisuals(UItem* Item, int Stack)
{
	CurrentItem = Item;
	SlotIconImage->SetVisibility(ESlateVisibility::Visible);
	SlotIconImage->SetBrushFromTexture(Item->ItemIcon);
	bIsOccupied = true;
}

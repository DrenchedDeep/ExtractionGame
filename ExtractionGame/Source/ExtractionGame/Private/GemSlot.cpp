


#include "GemSlot.h"

#include "InventoryWidget.h"

void UGemSlot::TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID)
{
	//check if item is a gem and if it is, transfer it to the gem slot
	UE_LOG(LogClass, Log, TEXT("My Int Value: %d"), TargetSlotID);
	Super::TransferSlots(SourceInventoryComponent, TargetSlotID);

	/*/
	if(const USlotWidget* OldSlot = SourceInventoryComponent->InventoryWidget->GetSlots()[TargetSlotID])
	{
		if(OldSlot->GetCurrentItem()->ItemType == EItemType::Gem)
		{
			Super::TransferSlots(SourceInventoryComponent, TargetSlotID);
		}
	}
	/*/
}

void UGemSlot::PredictVisuals(UItem* Item, int Stack)
{
	CurrentItem = Item;
	SlotIconImage->SetVisibility(ESlateVisibility::Visible);
	SlotIconImage->SetBrushFromTexture(Item->ItemIcon);
	bIsOccupied = true;
}

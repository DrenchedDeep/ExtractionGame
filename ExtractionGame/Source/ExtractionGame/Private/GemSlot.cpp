


#include "GemSlot.h"

#include "InventoryWidget.h"
#include "ExtractionGame/ExtractionGameCharacter.h"

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

void UGemSlot::ReconcileVisuals(const FInventoryItem& Item)
{
	Super::ReconcileVisuals(Item);

	if(AExtractionGameCharacter* Character = Cast<AExtractionGameCharacter>(Inventory->GetOwner()))
	{
		Character->GemController->Server_CreateGem(Item.ItemID, BodyPart, GetSlotID());
	}
}

void UGemSlot::Reset()
{
	UE_LOG(LogTemp, Warning, TEXT("Resetting Gem Slot"));
	if(!CurrentItem)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT(" hi"));

	if(AExtractionGameCharacter* Character = Cast<AExtractionGameCharacter>(Inventory->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT(" character!"));

		Character->GemController->RemoveGem(BodyPart);
	}

	Super::Reset();

}

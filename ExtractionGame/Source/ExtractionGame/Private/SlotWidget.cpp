


#include "SlotWidget.h"
#include "Item.h"
#include "Components/TextBlock.h"

void USlotWidget::Init(int ID)
{
	SlotID = ID;
	SlotIconImage->SetVisibility(ESlateVisibility::Hidden); 
	SlotStackText->SetVisibility(ESlateVisibility::Hidden);
}

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




#include "InventoryWidget.h"

#include "Blueprint/WidgetTree.h"


void UInventoryWidget::Init(int32 SlotSize)
{
	for(int i = 0; i < SlotSize; i++)
	{
		USlotWidget* InventorySlot = WidgetTree->ConstructWidget<USlotWidget>(SlotWidgetSubclass, TEXT("Slot " + i));
		InventorySlot->Init(i);
		
		InventoryGridPanel->AddChild(InventorySlot);
		Slots.Add(InventorySlot);
	}
}




#include "InventoryWidget.h"

#include "GemSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"


void UInventoryWidget::Init(UInventoryComponent* InventoryComponent, int32 SlotSize, bool bInitGemSlots)
{
	OwnerInventory = InventoryComponent;
	for(int i = 0; i < SlotSize; i++)
	{
		USlotWidget* InventorySlot = WidgetTree->ConstructWidget<USlotWidget>(SlotWidgetSubclass, TEXT("Slot " + i));
		InventorySlot->Init(InventoryComponent, i);
		
		InventoryGridPanel->AddChild(InventorySlot);
		Slots.Add(InventorySlot);
	}

	if(bInitGemSlots)
	{
		TArray<UUserWidget*> GemsSlots;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), GemsSlots, GemSlotWidgetSubclass, false);
		
		if(GemsSlots.Num() > 0)
		{
			for(int i = 0; i < GemsSlots.Num(); i++)
			{
				if(UGemSlot* GemSlot = Cast<UGemSlot>(GemsSlots[i]))
				{
					GemSlot->Init(InventoryComponent, 20 + i);
					Slots.Add(GemSlot);
				}
			}
		}
	}
}

bool UInventoryWidget::SetSlot(int SlotIndex, int ItemIndex, UInventoryComponent* Inventory)
{
	bool bSuccess = false;
		
	for(int i = 0; i < Slots.Num(); i++)
	{
			if(Slots[i]->GetSlotID() == SlotIndex)
			{
				if(Slots[i])
				{
				//	Slots[i]->InitItem(Inventory, ItemIndex);
				}
				bSuccess = true;
				break;
			}
	} 
		
	return bSuccess;
}

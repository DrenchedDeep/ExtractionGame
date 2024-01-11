


#include "UI/Widgets/InventoryWidget.h"

#include "ui/Widgets/GemSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"


void UInventoryWidget::Init(UInventoryComponent* InventoryComponent, int32 SlotSize, bool bInitGemSlots)
{
	OwnerInventory = InventoryComponent;

	if(!bInitialized)
	{
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
					//	GemSlot->Init(InventoryComponent, 20 + i);
					//	Slots.Add(GemSlot);
					}
				}
			}
		}
		bInitialized = true;
	}
	else
	{
		for(int i = 0; i < Slots.Num(); i++)
		{
			Slots[i]->Init(InventoryComponent, i);
		}
	}
}

void UInventoryWidget::Reset()
{
	for(int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->Reset();
	}
}
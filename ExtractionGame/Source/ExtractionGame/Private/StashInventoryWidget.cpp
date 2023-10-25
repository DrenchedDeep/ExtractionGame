#include "StashInventoryWidget.h"

#include "StashInventoryComponent.h"
#include "Blueprint/WidgetTree.h"


void UStashInventoryWidget::InitInventoryWidget(UStashInventoryComponent* InventoryComponent, int32 PlayerInventorySize,
	int32 StashInventorySize)
{
	StashGridPanel->Init(InventoryComponent, StashInventorySize);
	PlayerInventoryGridPanel->Init(InventoryComponent, PlayerInventorySize);
}

void UStashInventoryWidget::SaveInventory()
{
}




#include "ExtractionGameHUD.h"

UInventoryWidget* AExtractionGameHUD::CreateInventoryWidget()
{
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetSubclass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	return InventoryWidget;
}

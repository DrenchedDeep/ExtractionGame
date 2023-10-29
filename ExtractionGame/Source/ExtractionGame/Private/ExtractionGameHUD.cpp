


#include "ExtractionGameHUD.h"

UInventoryWidget* AExtractionGameHUD::CreateInventoryWidget()
{
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetSubclass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("hi"));
	return InventoryWidget;
}

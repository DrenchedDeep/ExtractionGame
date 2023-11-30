


#include "ExtractionGameHUD.h"

UInventoryWidget* AExtractionGameHUD::CreateInventoryWidget()
{
	if(InventoryWidget)
	{
		InventoryWidget->Reset();
		return InventoryWidget;
	}
	
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetSubclass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

	return InventoryWidget;
}

void AExtractionGameHUD::BeginPlay()
{
	Super::BeginPlay();

	DeathWidget = CreateWidget<UDeathWidget>(GetWorld(), DeathWidgetSubclass);
}

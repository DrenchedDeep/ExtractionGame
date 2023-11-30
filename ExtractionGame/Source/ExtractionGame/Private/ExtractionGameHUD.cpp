


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

UPlayerBarData* AExtractionGameHUD::CreatePlayerBarDataWidget()
{
	if(PlayerUIData)
	{
		return PlayerUIData;
	}
	
	PlayerUIData = CreateWidget<UPlayerBarData>(GetWorld(), PlayerUIDataSubclass);
	PlayerUIData->AddToViewport();
	PlayerUIData->SetVisibility(ESlateVisibility::Visible);

	return PlayerUIData;
}

void AExtractionGameHUD::BeginPlay()
{
	Super::BeginPlay();

	DeathWidget = CreateWidget<UDeathWidget>(GetWorld(), DeathWidgetSubclass);
}

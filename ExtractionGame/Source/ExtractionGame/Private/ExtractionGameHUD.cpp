


#include "ExtractionGameHUD.h"

UPlayerBarData* AExtractionGameHUD::GetPlayerBarWidget() const
{
	if(!PlayerUIData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget is null?"))
	}
	return PlayerUIData;
}

UInventoryWidget* AExtractionGameHUD::CreateInventoryWidget()
{
	if(InventoryWidget)
	{
		InventoryWidget->Reset();
		return InventoryWidget;
	}
	
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetSubclass);
	InventoryWidget->AddToViewport(100);
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

	return InventoryWidget;
}

void AExtractionGameHUD::CreatePlayerBarDataWidget()
{
	if(PlayerUIData)
	{
		return;
	}
	
	PlayerUIData = CreateWidget<UPlayerBarData>(GetWorld(), PlayerUIDataSubclass);
	PlayerUIData->AddToViewport();
	PlayerUIData->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Warning, TEXT("Widget is Built"))

}

void AExtractionGameHUD::BeginPlay()
{
	Super::BeginPlay();

	DeathWidget = CreateWidget<UDeathWidget>(GetWorld(), DeathWidgetSubclass);
	UE_LOG(LogTemp, Warning, TEXT("UI Loaded"))
	CreatePlayerBarDataWidget();
	
}

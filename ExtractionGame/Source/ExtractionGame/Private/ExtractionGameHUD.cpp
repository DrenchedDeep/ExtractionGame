


#include "ExtractionGameHUD.h"

UPlayerBarData* AExtractionGameHUD::GetPlayerBarWidget() const
{
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
	if(!PlayerUIData)
	{
		UE_LOG(LogTemp, Warning, TEXT("How's that possible? %s"), *PlayerUIDataSubclass->GetName())
		return;
	}	
	PlayerUIData->AddToViewport();

	PlayerUIData->SetVisibility(ESlateVisibility::Visible);
}

void AExtractionGameHUD::BeginPlay()
{
	Super::BeginPlay();

	DeathWidget = CreateWidget<UDeathWidget>(GetWorld(), DeathWidgetSubclass);
	UE_LOG(LogTemp, Warning, TEXT("UI Loaded"))
	//CreatePlayerBarDataWidget();
	
}

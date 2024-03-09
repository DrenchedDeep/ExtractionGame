


#include "UI/ExtractionGameHUD.h"

UPlayerBarDataWidget* AExtractionGameHUD::GetPlayerBarWidget() const
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
	
	PlayerUIData = CreateWidget<UPlayerBarDataWidget>(GetWorld(), PlayerUIDataSubclass);
	PlayerUIData->AddToViewport();
	PlayerUIData->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Warning, TEXT("HUD Loaded"))
}

void AExtractionGameHUD::BeginPlay()
{
	Super::BeginPlay();

	DeathWidget = CreateWidget<UDeathWidget>(GetWorld(), DeathWidgetSubclass);
	UE_LOG(LogTemp, Warning, TEXT("UI Loaded"))
	//CreatePlayerBarDataWidget();
	
}

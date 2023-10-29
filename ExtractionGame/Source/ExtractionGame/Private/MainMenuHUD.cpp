#include "MainMenuHUD.h"

void AMainMenuHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetSubclass);
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	
	RaidOverWidget = CreateWidget<URaidOverWidget>(GetWorld(), RaidOverWidgetSubclass);
	RaidOverWidget->AddToViewport();
	RaidOverWidget->SetVisibility(ESlateVisibility::Hidden);
	
	CreateNetworkErrorWidget();
}

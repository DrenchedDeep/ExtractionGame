#include "MainMenuHUD.h"

void AMainMenuHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetSubclass);
	MainMenuWidget->AddToViewport();
}

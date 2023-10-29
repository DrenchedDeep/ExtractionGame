


#include "RaidOverWidget.h"

#include "MainMenuHUD.h"

void URaidOverWidget::Next()
{
	const AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	MainMenuHUD->RaidOverWidget->SetVisibility(ESlateVisibility::Hidden);
	MainMenuHUD->MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void URaidOverWidget::Previous()
{
}

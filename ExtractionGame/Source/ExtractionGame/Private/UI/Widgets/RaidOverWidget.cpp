


#include "UI/Widgets/RaidOverWidget.h"

#include "UI/MainMenuHUD.h"

void URaidOverWidget::Next()
{
	const AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	MainMenuHUD->RaidOverWidget->SetVisibility(ESlateVisibility::Hidden);
	MainMenuHUD->MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void URaidOverWidget::Previous()
{
}

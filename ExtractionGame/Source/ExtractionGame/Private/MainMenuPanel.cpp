


#include "MainMenuPanel.h"

void UMainMenuPanel::EnableUI(bool bEnableUI)
{
	bIsShowing = true;
	
	if(bEnableUI)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuPanel::DisableUI(bool bDisableUI)
{
	bIsShowing = false;
	
	if(bDisableUI)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

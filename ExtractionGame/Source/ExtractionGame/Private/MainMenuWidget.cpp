


#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EnablePanelByID("MainLobby");
}

void UMainMenuWidget::DisableAllPanels()
{
	for(int i = 0; i < Panels.Num(); i++)
	{
		Panels[i]->DisableUI();
	}
}

void UMainMenuWidget::EnablePanelByID(FName PanelName)
{
	UMainMenuPanel* NextPanel;
	bool bSuccess = GetPanelByID(PanelName, NextPanel);
	
	if(bSuccess && NextPanel)
	{
		DisableAllPanels();
		NextPanel->EnableUI();
		CurrentPanel = NextPanel;
	}
}

void UMainMenuWidget::DisablePanelByID(FName PanelName)
{
	UMainMenuPanel* NextPanel;
	bool bSuccess = GetPanelByID(PanelName, NextPanel);
	
	if(bSuccess && NextPanel)
	{
		NextPanel->DisableUI();
	}
}

bool UMainMenuWidget::GetPanelByID(FName ID, UMainMenuPanel*& Panel)
{
	bool bSuccess = false;
	Panel = nullptr;

	for(int i = 0; i < Panels.Num(); i++)
	{	
		if(ID == Panels[i]->PanelName)
		{
			Panel = Panels[i]; 
			bSuccess = true;
			break;
		}
	}

	return bSuccess;
}


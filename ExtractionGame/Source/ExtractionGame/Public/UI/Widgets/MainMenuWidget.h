

#pragma once

#include "CoreMinimal.h"
#include "UI/MainMenuPanel.h"
#include "StashInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UMainMenuPanel*> Panels;

	UPROPERTY(BlueprintReadOnly)
	UMainMenuPanel* CurrentPanel;

	UFUNCTION(BlueprintCallable)
	void DisableAllPanels();
	UFUNCTION(BlueprintCallable)
	void EnablePanelByID(FName PanelName);
	UFUNCTION(BlueprintCallable)
	void DisablePanelByID(FName PanelName);

private:
	bool GetPanelByID(FName ID, UMainMenuPanel*& Panel);
};

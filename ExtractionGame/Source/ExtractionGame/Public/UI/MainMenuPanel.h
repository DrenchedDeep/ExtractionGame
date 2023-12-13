

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuPanel.generated.h"

UCLASS()
class EXTRACTIONGAME_API UMainMenuPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PanelName;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsShowing;
	
	UFUNCTION(BlueprintCallable)
	virtual void EnableUI(bool bEnableUI = true);
	UFUNCTION(BlueprintCallable)
	virtual void DisableUI(bool bDisableUI = true);
};

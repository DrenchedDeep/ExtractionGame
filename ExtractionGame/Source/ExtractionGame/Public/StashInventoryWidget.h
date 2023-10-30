

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "MainMenuPanel.h"
#include "StashWidget.h"
#include "StashInventoryWidget.generated.h"

class APlayerStashManager;
class UStashInventoryComponent;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UStashInventoryWidget : public UMainMenuPanel
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UInventoryWidget* StashGridPanel;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UInventoryWidget* PlayerInventoryGridPanel;

	
	void InitInventoryWidget(UStashInventoryComponent* InventoryComponent, int32 PlayerInventorySize, int32 StashInventorySize);

	virtual void NativeConstruct() override;
	virtual void EnableUI(bool bEnableUI) override;
	virtual void DisableUI(bool bDisableUI) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemHovered(UItem* Item);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemUnHovered();
	
	void OnInventorySaved();
	void OnInventoryLoaded();
};


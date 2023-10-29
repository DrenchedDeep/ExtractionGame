

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "StashWidget.h"
#include "StashInventoryWidget.generated.h"

class APlayerStashManager;
class UStashInventoryComponent;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UStashInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UInventoryWidget* StashGridPanel;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UInventoryWidget* PlayerInventoryGridPanel;

	
	void InitInventoryWidget(UStashInventoryComponent* InventoryComponent, int32 PlayerInventorySize, int32 StashInventorySize);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent) void ToggleInventory(bool bToggle);
	UFUNCTION(BlueprintImplementableEvent) void ToggleLoadingScreen(bool bToggle);
	UFUNCTION(BlueprintImplementableEvent) void ToggleSavingScreen(bool bToggle);

	UPROPERTY(BlueprintReadOnly);
	APlayerStashManager* PlayerStashManager;

	void OnInventorySaved();
	void OnInventoryLoaded();
};


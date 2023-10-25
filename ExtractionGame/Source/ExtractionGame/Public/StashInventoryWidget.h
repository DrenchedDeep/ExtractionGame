

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "StashWidget.h"
#include "StashInventoryWidget.generated.h"

class UStashInventoryComponent;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UStashInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryWidget* StashGridPanel;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryWidget* PlayerInventoryGridPanel;

	
	void InitInventoryWidget(UStashInventoryComponent* InventoryComponent, int32 PlayerInventorySize, int32 StashInventorySize);
	void SaveInventory();
};

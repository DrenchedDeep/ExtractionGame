

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SlotWidget.generated.h"

class UItem;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* SlotIconImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotStackText;
	
public:

	UPROPERTY(BlueprintReadOnly)
	bool bIsOccupied;
	
	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* Inventory;

	
	void Init(int ID);
	void InitItem(UInventoryComponent* InventoryComponent, int InvenIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetSlotID() const { return SlotID;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UItem* GetCurrentItem() const
	{
		if(!Inventory || !Inventory->InventoryItems[InventoryIndex].ItemID)
		{
			return nullptr;
		}

		return Inventory->InventoryItems[InventoryIndex].ItemID;
	}

	
private:
	int SlotID;
	int InventoryIndex;
};

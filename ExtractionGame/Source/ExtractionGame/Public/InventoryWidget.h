

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "SlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "InventoryWidget.generated.h"


//handles all of the slots
UCLASS()
class EXTRACTIONGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryGridPanel;
	UPROPERTY(EditAnywhere,  meta=(AllowPrivateAccess = "true"))
	TSubclassOf<USlotWidget> SlotWidgetSubclass;
	
public:
	UFUNCTION(BlueprintCallable)
	void Init(int32 SlotSize);

	FORCEINLINE USlotWidget* GetSlot(int Index)
	{
		USlotWidget* InventorySlot = nullptr;

		for(int i = 0; i < Slots.Num(); i++)
		{
			if(Index == Slots[i]->GetSlotID())
			{
				InventorySlot = Slots[i];
			}
		}
		
		return InventorySlot;
	}

	FORCEINLINE bool SetSlot(int Index, FInventoryItem InventoryItem)
	{
		bool bSuccess = false;
		
		if(Slots[Index] != nullptr)
		{
			Slots[Index]->InitItem(InventoryItem.ItemID, InventoryItem.StackSize);
			bSuccess = true;
		}

		return bSuccess;
	}

	FORCEINLINE USlotWidget* GetNextSlot()
	{
		USlotWidget* InventorySlot = nullptr;
		
		for(int i = 0; i < Slots.Num(); i++)
		{
			if(Slots[i]->bIsOccupied)
			{
				continue;
			}

			InventorySlot = Slots[i];
			break;
		}

		return InventorySlot;
	}
	
private:
	UPROPERTY()
	TArray<USlotWidget*> Slots;
	
};



#pragma once

#include "CoreMinimal.h"
#include "GemSlot.h"
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

public:
	UFUNCTION(BlueprintCallable)
	virtual void Init(UInventoryComponent* InventoryComponent, int32 SlotSize, bool bInitGemSlots = true);

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* OwnerInventory;
	
	UPROPERTY(EditAnywhere,  meta=(AllowPrivateAccess = "true"))
	TSubclassOf<USlotWidget> SlotWidgetSubclass;
	
	UPROPERTY(EditAnywhere,  meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UGemSlot> GemSlotWidgetSubclass;

	
	TArray<USlotWidget*> GetSlots() const { return Slots;};
	
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

	bool SetSlot(int SlotIndex, int ItemIndex, UInventoryComponent* Inventory);

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


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemHovered(UItem* Item);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemUnHovered();
	UFUNCTION(BlueprintImplementableEvent)
	void InitGemSlots(UInventoryComponent* InventoryComponent);

private:
	UPROPERTY()
	TArray<USlotWidget*> Slots;
	UPROPERTY()
	TArray<USlotWidget*> GemSlots;
};

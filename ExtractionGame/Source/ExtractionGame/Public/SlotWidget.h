

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


public:
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsOccupied;
	
	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* Inventory;
	
	void Init(UInventoryComponent* InventoryComponent,int ID);
	void Reset();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetSlotID() const { return SlotID;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetInventoryIndex() const { return InventoryIndex;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItem* GetCurrentItem() const { return CurrentItem;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetCurrentStack() const
	{
		constexpr int Stack = 0;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), InventoryIndex));

		for(int i = 0; i < Inventory->InventoryItems.Num(); i++)
		{
			if(InventoryIndex == Inventory->InventoryItems[i].InventoryID)
			{
				return Inventory->InventoryItems[i].StackSize;
			}
		}
		
		return Stack;
	}

	void SetInventoryIndex(const int Index) { InventoryIndex = Index;}
	
	UFUNCTION(BlueprintCallable)
	virtual void TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID);

	virtual void PredictVisuals(UItem* Item, int Stack);
	virtual void ReconcileVisuals(const FInventoryItem& Item);

	
private:
	int SlotID;
	int InventoryIndex;
	

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* SlotIconImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotStackText;
	UPROPERTY()
	UItem* CurrentItem;

};
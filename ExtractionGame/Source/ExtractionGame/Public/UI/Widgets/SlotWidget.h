

#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
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
	virtual void Reset();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetSlotID() const { return SlotID;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetInventoryIndex() const { return InventoryIndex;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItem* GetCurrentItem() const { return CurrentItem;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetCurrentStack() const
	{
		return Inventory->GetInventoryItem(GetInventoryIndex()).StackSize;
	}

	void SetInventoryIndex(const int Index) { InventoryIndex = Index;}
	
	UFUNCTION(BlueprintCallable)
	virtual void TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID);

	virtual void PredictVisuals(UItem* Item, int Stack);
	virtual void ReconcileVisuals(FInventoryItem& Item);

	
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


#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AExtractionGameCharacter;
class UInventoryWidget;
class USlotWidget;

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY()
	int InventoryID;
	UPROPERTY()
	int SlotID;
	UPROPERTY()
	int StackSize;
	UPROPERTY()
	UItem* ItemID;
	//ignore in most cases
	UPROPERTY()
	int GemSlotID;

	FInventoryItem(int InventoryID, int SlotID, int StackSize, UItem* ItemID)
		: InventoryID(InventoryID),
		  SlotID(SlotID),
		  StackSize(StackSize),
		  ItemID(ItemID)
	{
	}

	FInventoryItem() = default;
};

USTRUCT(BlueprintType)
struct FStartingItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	int Stack;
	UPROPERTY(EditDefaultsOnly)
	UItem* ItemID;

	FStartingItem(int Stack, UItem* ItemID)
		: Stack(Stack),
		  ItemID(ItemID)
	{
	}

	FStartingItem() = default;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly)
	TArray<FStartingItem> StartingItems;

	UPROPERTY(EditDefaultsOnly,
		meta=(ToolTip = "When the server updates the inventory, do we want to reconcile the visuals with the updated data or keep the predicted visuals?"))
	bool bReconcileVisuals;
	
	UFUNCTION()
	virtual void OnRep_InventoryItems();

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(UItem* Item, int StackSize, bool bClientSimulation, int SlotID = -1);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItem* Item, int StackSize, bool bClientSimulation);
	virtual void RemoveItem(int InventoryID, int StackSize = 1);
	virtual void TransferSlots(USlotWidget* OldSlot, USlotWidget* NewSlot);

	virtual FInventoryItem GetInventoryItem(int InventoryID);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_InventoryItems)
	TArray<FInventoryItem> InventoryItems;
	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* InventoryWidget;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_AddItem(UItem* Item, int StackSize, int InventoryIndex, int SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_RemoveItem(UItem* Item, int StackSize);
	UFUNCTION(Server, Reliable)
	virtual void Server_TransferSlots(int TargetInventoryItemID, int NewSlotID);
};

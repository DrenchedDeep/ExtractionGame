

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

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

	UPROPERTY(EditDefaultsOnly)
	TArray<FStartingItem> StartingItems;

	UPROPERTY(EditDefaultsOnly,
		meta=(ToolTip = "When the server updates the inventory, do we want to reconcile the visuals with the updated data or keep the predicted visuals?"))
	bool bReconcileVisuals;

public:	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_InventoryItems();

	UFUNCTION(BlueprintCallable)
	void AddItem(UItem* Item, int StackSize, bool bClientSimulation);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItem* Item, int StackSize, bool bClientSimulation);
	void TransferSlots(USlotWidget* OldSlot, USlotWidget* NewSlot);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_InventoryItems)
	TArray<FInventoryItem> InventoryItems;
	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* InventoryWidget;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY()
	AExtractionGameCharacter* Character;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItem(UItem* Item, int StackSize, int InventoryIndex, int SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(UItem* Item, int StackSize);
	UFUNCTION(Server, Reliable)
	void Server_TransferSlots(int TargetInventoryItemID, int NewSlotID);
};

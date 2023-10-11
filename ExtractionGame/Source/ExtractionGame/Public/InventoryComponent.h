

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	int SlotID;
	UPROPERTY()
	int StackSize;
	UPROPERTY()
	UItem* ItemID;

	FInventoryItem(int SlotID, int StackSize, UItem* ItemID)
		: SlotID(SlotID),
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

	bool bInitialized;
	
public:	
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

	void AddItem(UItem* ItemData, int Stack = 1);
	void RemoveItem(UItem* ItemData, int Stack = 1);
	void RemoveItem(int SlotID, int Stack = 1);

	FInventoryItem FindItem(UItem* Item);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	TArray<FInventoryItem> InventoryItems;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	AExtractionGameCharacter* Character;
};

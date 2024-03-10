

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComp.generated.h"


class AItemActor;
class AExtractionGameCharacter;
class UItemObject;
enum EGemType : uint8;

UENUM(BlueprintType)
enum EItemStat
{
	ET_Heal,
	ET_Poison,
	ET_Misc,
	ET_SpeedBoost,
	ET_Speed,
	ET_Damage,
	ET_AOEDamage,
	ET_SpeedDebuff,
	ET_FireRate,
	ET_Ammo,
	Purity,
	Knockback
};

USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EItemStat> Stat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPointToVariable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCombineWith;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString VariableName;
};



USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	int32 X;
	UPROPERTY(BlueprintReadWrite)
	int32 Y;
};

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector2D Start;
	UPROPERTY(BlueprintReadOnly)
	FVector2D End;
};

USTRUCT(BlueprintType)
struct FItemIndex
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bValid;
	UPROPERTY(BlueprintReadOnly)
	UItemObject* Item;
};

UENUM(BlueprintType)
enum EItemTypes : uint8
{
	Junk = 0 UMETA(DisplayName = "Junk"),
	Gem = 1 UMETA(DisplayName = "Gem"),
	StoryItem = 2 UMETA(DisplayName = "StoryItem"),
	Food = 3 UMETA(DisplayName = "Food"),
	Utility = 4 UMETA(DisplayName = "Utility"),
	Currency = 5 UMETA(DisplayName = "Currency"),
	Luxury = 6 UMETA(DisplayName = "Luxury"),
};

UENUM(BlueprintType)
enum ERarityType : uint8
{
	Common = 0 UMETA(DisplayName = "Common"),
	Uncommon = 1 UMETA(DisplayName = "Uncommon"),
	Rare = 2 UMETA(DisplayName = "Rare"),
	Epic = 3 UMETA(DisplayName = "Epic"),
	Legendary = 4 UMETA(DisplayName = "Legendary"),
	NEVER = 5 UMETA(DisplayName = "NEVER"),
	QuestReward = 6 UMETA(DisplayName = "QuestReward"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	FString ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="General")
	int32 ItemCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	TEnumAsByte<ERarityType> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	int32 SizeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	int32 SizeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	TArray<UMaterialInterface*> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	UMaterialInterface* IconRotated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	TSubclassOf<AItemActor> ItemActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	TArray<FItemStat> ItemStats;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsCrushable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsUseable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsCombineable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gems")
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gems")
	float DefaultPolish;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combining")
	int32 ItemsNeededToCombine = 3;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combining")
	FName CombinedItemRowName;
};

USTRUCT(BlueprintType, Blueprintable)
struct FAddItemInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector2D Dimensions;
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* Icon;
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* IconRotated;
	UPROPERTY(BlueprintReadWrite)
	int32 ItemCost;
	UPROPERTY(BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ERarityType> Rarity;
	UPROPERTY(BlueprintReadWrite)
	FString Description;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(BlueprintReadWrite)
	float DefaultPolish;
	UPROPERTY(BlueprintReadWrite)
	FName RowName;
};


UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOInventoryChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UInventoryComp : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Columns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Rows;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TileSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString InventoryID = "Inventory";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bOnlyAllowItemsFromInventoryWithID = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString AllowedInventoryID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bBlockDragDrop = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bShowPriceUI = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bBlockItemActionsButKeepExamine;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* InventoryWidget;
public:
	UInventoryComp();
	
	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemByName(const FName& ItemRowName, int32 AmountToRemove);
	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItemByAddItemInfo(FAddItemInfo Item);
	
	UFUNCTION(BlueprintCallable)
	virtual void AddItem(int32 Index, UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	virtual void SetInventoryWidget(UUserWidget* Widget);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FTile IndexToTile(int32 Index);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 TileToIndex(FTile Tile);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<UItemObject*, FTile> GetAllItems();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool HasRoomForItem(FVector2D Dimensions, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	virtual TArray<FAddItemInfo> GetItemsAsAddItemInfo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FItemIndex GetItemAtIndex(int32 InIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsTileValid(FTile Tile);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 GetInventoryTotalCost();
	
	UPROPERTY(BlueprintAssignable)
	FOInventoryChangedDelegate OnInventoryChanged;

	UFUNCTION(BlueprintCallable)
	void AddItemDirectly(UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItemDirectly(UItemObject* Item);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_Rotate(UItemObject* ItemObject);

	UFUNCTION(BlueprintCallable)
	bool HasItems(FName ItemRowName, int32 AmountNeeded);
	
	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(FName InItemName);

	TMap<int32,FAddItemInfo> GetPlayerInventory();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	UFUNCTION(Server, Reliable)
	virtual  void Server_AddItemByObject(UItemObject* Item, int32 Index);
	UFUNCTION(Server, Reliable)
	virtual  void Server_AddItem(FAddItemInfo ItemInfo, int32 Index);
	UFUNCTION(Server, Reliable)
	virtual void Server_RemoveItem(UItemObject* Item);
	UFUNCTION(Server, Reliable)
	virtual void Server_AddItemDirectly(UItemObject* Item);
	UFUNCTION(Server, Reliable)
	virtual void Server_RemoveItemDirectly(UItemObject* Item);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemObject> ItemObjectSubclass;
	
	UFUNCTION()
	virtual void OnRep_Items();

	UPROPERTY(ReplicatedUsing=OnRep_Items)
	TArray<UItemObject*> Items;

	UPROPERTY()
	AExtractionGameCharacter* Character;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsDirty;
	
};

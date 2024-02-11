

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
	ET_SpeedBoost
};

USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EItemStat> Stat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERarityType> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* IconRotated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemActor> ItemActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemStat> ItemStats;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsCrushable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsUseable;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultPolish;

	UPROPERTY(BlueprintReadWrite)
	bool bFoundInRaid;
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
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* InventoryWidget;
public:
	UInventoryComp();
	
	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItemObject* Item);

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
	
	UPROPERTY(BlueprintAssignable)
	FOInventoryChangedDelegate OnInventoryChanged;

	UFUNCTION(BlueprintCallable)
	void AddItemDirectly(UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItemDirectly(UItemObject* Item);
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

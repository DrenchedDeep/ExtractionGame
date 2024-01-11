

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComp.generated.h"


class UItemObject;
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
	virtual bool HasRoomForItem(UItemObject* Item, int32 TopLeftIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FItemIndex GetItemAtIndex(int32 InIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsTileValid(FTile Tile);
	
	UPROPERTY(BlueprintAssignable)
	FOInventoryChangedDelegate OnInventoryChanged; 
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<UItemObject*> Items;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsDirty;
};




#include "Components/InventoryComp.h"

#include "Components/ItemObject.h"


UInventoryComp::UInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(Columns * Rows);
}

void UInventoryComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bIsDirty)
	{
		bIsDirty = false;
		OnInventoryChanged.Broadcast();
	}
}


bool UInventoryComp::TryAddItem(UItemObject* Item)
{
	bool bSuccess = false;
	
	if(!Item)
	{
		return bSuccess;
	}


	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(HasRoomForItem(Item, i))
		{
			AddItem(i, Item);
			bSuccess = true;
			break;
		}
	}

	return bSuccess;
}

void UInventoryComp::RemoveItem(UItemObject* Item)
{
	if(!Item)
	{
		return;
	}

	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(Items[i] == Item)
		{
			Items[i] = nullptr;
			bIsDirty = true;
		}
	}
}

TMap<UItemObject*, FTile> UInventoryComp::GetAllItems()
{
	TMap<UItemObject*, FTile> ItemMap;
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(!Items[i])
		{
			continue;
		}

		if(ItemMap.Contains(Items[i]))
		{
			continue;
		}

		ItemMap.Add(Items[i], IndexToTile(i));
	}

	return ItemMap;
}

bool UInventoryComp::HasRoomForItem(UItemObject* Item, int32 TopLeftIndex)
{
	FTile Tile = IndexToTile(TopLeftIndex);
	for(int32 i = Tile.X; i < Tile.X + Item->GetDimensions().X; i++)
	{
		for(int32 z = Tile.Y; z < Tile.Y + Item->GetDimensions().Y; z++)
		{
			FTile CurrentTile = {i, z};

			if(!IsTileValid(CurrentTile))
			{
				UE_LOG(LogTemp, Warning, TEXT("00"));
				return false;
			}

			const int32 TileIndex = TileToIndex(CurrentTile);
			const FItemIndex ItemIndex = GetItemAtIndex(TileIndex);

			if(!ItemIndex.bValid)
			{
				UE_LOG(LogTemp, Warning, TEXT("cc"));
				return false;
			}
			
			if(ItemIndex.Item)
			{
				UE_LOG(LogTemp, Warning, TEXT("aa"));
				return false;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("SUCCESS"));
	return true;
}

FTile UInventoryComp::IndexToTile(int32 Index)
{
	const FTile Tile { Index % Columns, Index / Columns };

	return Tile;
}

int32 UInventoryComp::TileToIndex(FTile Tile)
{
	return Tile.X + Tile.Y * Columns;
}

FItemIndex UInventoryComp::GetItemAtIndex(int32 InIndex)
{
	if(!Items.IsValidIndex(InIndex))
	{
		return FItemIndex();
	}

	return FItemIndex{true, Items[InIndex] };
}

void UInventoryComp::AddItem(int32 Index, UItemObject* Item)
{
	FTile Tile = IndexToTile(Index);
	for(int32 i = Tile.X; i < Tile.X + Item->GetDimensions().X; i++)
	{
		for(int32 z = Tile.Y; z < Tile.Y + Item->GetDimensions().Y; z++)
		{
			int32 I = TileToIndex({i, z});
			Items[I] = Item;
			UE_LOG(LogTemp, Warning, TEXT("Item added to index %d"), I);
		}
	}

	bIsDirty = true;
}

void UInventoryComp::SetInventoryWidget(UUserWidget* Widget)
{
	InventoryWidget = Widget;
}

bool UInventoryComp::IsTileValid(FTile Tile)
{
	bool bValid = false;
	
	if(Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows)
	{
		bValid = true;
	}

	return bValid;
}

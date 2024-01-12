


#include "Components/InventoryComp.h"

#include "Components/ItemObject.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Objects/ReplicatedItemObject.h"


UInventoryComp::UInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AExtractionGameCharacter>(GetOwner());

	Items.SetNum(Columns * Rows);

}

void UInventoryComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Character && Character->IsLocallyControlled())
	{
		if(bIsDirty)
		{
			UE_LOG(LogTemp, Warning, TEXT("bIsDirty"));
			bIsDirty = false;
			OnInventoryChanged.Broadcast();
		}
	}
}

void UInventoryComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//only replicate to the owner of the inventory
	DOREPLIFETIME(UInventoryComp, Items);
}

bool UInventoryComp::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	//return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(Items[i])
		{
			bWroteSomething |= Channel->ReplicateSubobject(Items[i], *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UInventoryComp::Server_RemoveItem_Implementation(UItemObject* Item)
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
			OnRep_Items();
		}
	}
}

void UInventoryComp::Server_AddItem_Implementation(FAddItemInfo ItemInfo, int32 Index)
{
	UReplicatedItemObject* ItemObject = NewObject<UReplicatedItemObject>(	this,ItemObjectSubclass);
			
	ItemObject->ItemName = ItemInfo.ItemName;
	ItemObject->ItemType = ItemInfo.ItemType;
	ItemObject->Rarity = ItemInfo.Rarity;
	ItemObject->Description = ItemInfo.Description;
	ItemObject->GemType = ItemInfo.GemType;
	ItemObject->DefaultPolish = ItemInfo.DefaultPolish;
	ItemObject->Dimensions = ItemInfo.Dimensions;
	ItemObject->Icon = ItemInfo.Icon;
	ItemObject->IconRotated = ItemInfo.IconRotated;

	FTile Tile = IndexToTile(Index);
	for(int32 i = Tile.X; i < Tile.X + ItemInfo.Dimensions.X; i++)
	{
		for(int32 z = Tile.Y; z < Tile.Y + ItemInfo.Dimensions.Y; z++)
		{
			int32 I = TileToIndex({i, z});
			Items[I] = ItemObject;
		}
	}

	OnRep_Items();
	//Client_AddItem();

}

void UInventoryComp::Client_AddItem_Implementation()
{
	bIsDirty = true;
}

void UInventoryComp::OnRep_Items()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Items"));
	bIsDirty = true;
}

void UInventoryComp::Server_AddItemByObject_Implementation(UItemObject* Item, int32 Index)
{
	FTile Tile = IndexToTile(Index);
	for(int32 i = Tile.X; i < Tile.X + Item->GetDimensions().X; i++)
	{
		for(int32 z = Tile.Y; z < Tile.Y + Item->GetDimensions().Y; z++)
		{
			int32 I = TileToIndex({i, z});
			Items[I] = Item;
			
		}
	}

	OnRep_Items();
	//Client_AddItem();
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
		if(HasRoomForItem(Item->GetDimensions(), i))
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
	Server_RemoveItem(Item);

}

bool UInventoryComp::TryAddItemByAddItemInfo(FAddItemInfo Item)
{
	UE_LOG(LogTemp, Warning, TEXT("TryAddItemByAddItemInfo"));
	bool bSuccess = false;

	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(HasRoomForItem(FVector2D(Item.Dimensions.X, Item.Dimensions.Y), i))
		{
			Server_AddItem(Item, i);
			return true;
		}
	}

	return bSuccess;
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

bool UInventoryComp::HasRoomForItem(FVector2D Dimensions, int32 TopLeftIndex)
{
	FTile Tile = IndexToTile(TopLeftIndex);
	for(int32 i = Tile.X; i < Tile.X + Dimensions.X; i++)
	{
		for(int32 z = Tile.Y; z < Tile.Y + Dimensions.Y; z++)
		{
			FTile CurrentTile = {i, z};

			if(!IsTileValid(CurrentTile))
			{
				return false;
			}

			const int32 TileIndex = TileToIndex(CurrentTile);
			const FItemIndex ItemIndex = GetItemAtIndex(TileIndex);

			if(!ItemIndex.bValid)
			{
				return false;
			}
			
			if(ItemIndex.Item)
			{
				return false;
			}
		}
	}
	
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
	if(Character && Character->HasAuthority())
	{
		FTile Tile = IndexToTile(Index);
		for(int32 i = Tile.X; i < Tile.X + Item->GetDimensions().X; i++)
		{
			for(int32 z = Tile.Y; z < Tile.Y + Item->GetDimensions().Y; z++)
			{
				int32 I = TileToIndex({i, z});
				Items[I] = Item;
				UE_LOG(LogTemp, Warning, TEXT("Item added to index %d"), I);
				OnRep_Items();
			}
		}
	}
	else
	{
		Server_AddItemByObject(Item, Index);
	}
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

#include "Components/StashInventoryComponent.h"

#include "Components/ItemObject.h"
#include "UI/MainMenuHUD.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"



void UStashInventoryComponent::AddItem(int32 Index, UItemObject* Item)
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

void UStashInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bIsDirty)
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsDirty"));
		bIsDirty = false;
		OnInventoryChanged.Broadcast();
	}
}




#include "Components/PlayerInventoryComponent.h"

#include "Components/ItemObject.h"
#include "UI/ExtractionGameHUD.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Objects/ReplicatedItemObject.h"


class UReplicatedItemObject;

void UPlayerInventoryComponent::InitStartingItems()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());

	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}
	
	if(GameInstance)
	{
		for(auto Item : GameInstance->PlayerSessionData.PlayerItems)
		{
			Server_AddItem(Item.Value, Item.Key);
		}

		for(auto Gem : GameInstance->PlayerSessionData.GemItems)
		{
			Server_AddGemRaw(Gem.Value, Gem.Key);
		}
	}
}

void UPlayerInventoryComponent::Server_AddGem_Implementation(UItemObject* Item, EBodyPart BodyPart)
{
	GemItems.Add(FGemItem{Item, BodyPart});
	OnRep_GemItems();
}

void UPlayerInventoryComponent::Server_AddGemRaw_Implementation(FAddItemInfo Item, EBodyPart BodyPart)
{
	UReplicatedItemObject* ItemObject = NewObject<UReplicatedItemObject>(	this,ItemObjectSubclass);
			
	ItemObject->ItemName = Item.ItemName;
	ItemObject->ItemType = Item.ItemType;
	ItemObject->Rarity = Item.Rarity;
	ItemObject->Description = Item.Description;
	ItemObject->GemType = Item.GemType;
	ItemObject->DefaultPolish = Item.DefaultPolish;
	ItemObject->Dimensions = Item.Dimensions;
	ItemObject->Icon = Item.Icon;
	ItemObject->IconRotated = Item.IconRotated;
	ItemObject->RowName = Item.RowName;

	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}

	if(Character)
	{
		Character->GemController->Server_CreateGem(ItemObject, BodyPart);
	}

	GemItems.Add(FGemItem{ItemObject, BodyPart});
	OnRep_GemItems();
}



TArray<UItemObject*> UPlayerInventoryComponent::GetItemsIncludingGems()
{
	TArray<UItemObject*> Itms;

	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(!Items[i])
		{
			continue;
		}

		if(Itms.Contains(Items[i]))
		{
			continue;
		}

		Itms.Add(Items[i]);
	}


	for(int32 i = 0; i < GemItems.Num(); i++)
	{
		if(!GemItems[i].Item)
		{
			continue;
		}

		if(Itms.Contains(GemItems[i].Item))
		{
			continue;
		}

		Itms.Add(GemItems[i].Item);
	}
	
	return Itms;
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPlayerInventoryComponent, GemItems, COND_OwnerOnly);
}

void UPlayerInventoryComponent::OnRep_GemItems()
{
	if(Character)
	{
		Character->InitGemUI();
	}
}


void UPlayerInventoryComponent::Server_RemoveGem_Implementation(UItemObject* Item)
{
	for(auto Gem : GemItems)
	{
		/*/
		UItemObject* GemItem = Gem.Item;
		if(Item == GemItem)
		{
			GemItems.Remove(Gem);
			break;
		}
		/*/
	}
}

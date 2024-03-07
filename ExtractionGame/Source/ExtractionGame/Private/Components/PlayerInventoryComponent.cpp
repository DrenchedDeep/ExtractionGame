


#include "Components/PlayerInventoryComponent.h"

#include "Components/ItemObject.h"
#include "UI/ExtractionGameHUD.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Engine/ActorChannel.h"
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
		UE_LOG(LogTemp, Warning, TEXT("InitStartingItems"));
		for(auto Item : GameInstance->PlayerSessionData.PlayerItems)
		{
			Server_AddItem(Item.Value, Item.Key);
			UE_LOG(LogTemp, Warning, TEXT("Adding Item: %s"), *Item.Value.ItemName);
		}

		for(auto Gem : GameInstance->PlayerSessionData.GemItems)
		{
			Server_AddGemRaw(Gem.Value, Gem.Key);
			UE_LOG(LogTemp, Warning, TEXT("Adding Gem: %s"), *Gem.Value.ItemName);
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

TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> UPlayerInventoryComponent::GetGemInventory()
{
	TMap<TEnumAsByte<EBodyPart>,FAddItemInfo> Gems;

	for(auto Item :  GemItems)
	{
		FAddItemInfo Info;
		Info.Description = Item.Item->Description;
		Info.Dimensions = Item.Item->Dimensions;
		Info.Icon = Item.Item->Icon;
		Info.IconRotated =Item.Item->IconRotated;
		Info.Rarity = Item.Item->Rarity;
		Info.ItemName = Item.Item->ItemName;
		Info.ItemType = Item.Item->ItemType;
		Info.GemType = Item.Item->GemType;
		Info.DefaultPolish = Item.Item->DefaultPolish;

		Gems.Add(Item.BodyPart, Info);
	}

	return Gems;
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInventoryComponent, GemItems);
}

bool UPlayerInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool BSuccess = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	for(int32 i = 0; i < GemItems.Num(); i++)
	{
		if(GemItems[i].Item)
		{
			BSuccess |= Channel->ReplicateSubobject(GemItems[i].Item, *Bunch, *RepFlags);
		}
	}

	return BSuccess;
}

void UPlayerInventoryComponent::OnRep_GemItems()
{
	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}
	
	if(Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitGemUI"));
		Character->InitGemUI();
	}
}


void UPlayerInventoryComponent::Server_RemoveGem_Implementation(UItemObject* Item)
{
	for(int32 i = 0; i < GemItems.Num(); i++)
	{
		if(GemItems[i].Item == Item)
		{
			GemItems.RemoveAt(i);
			OnRep_GemItems();
			return;
		}
	}
}

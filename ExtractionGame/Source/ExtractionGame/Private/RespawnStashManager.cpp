


#include "RespawnStashManager.h"

#include "Components/ItemObject.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"

TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> ARespawnStashManager::GetGemInventory()
{
	TMap<TEnumAsByte<EBodyPart>,FAddItemInfo> GemItems;

	for(auto Item : GemInventory)
	{
		FAddItemInfo Info;
		Info.Description = Item.Value->Description;
		Info.Dimensions = Item.Value->Dimensions;
		Info.Icon = Item.Value->Icon;
		Info.IconRotated = Item.Value->IconRotated;
		Info.Rarity = Item.Value->Rarity;
		Info.ItemName = Item.Value->ItemName;
		Info.ItemType = Item.Value->ItemType;
		Info.GemType = Item.Value->GemType;
		Info.DefaultPolish = Item.Value->DefaultPolish;
		Info.RowName = Item.Value->RowName;

		GemItems.Add(Item.Key, Info);
	}

	return GemItems;
}

void ARespawnStashManager::ResetGemInventory()
{
	GemInventory.Reset();
	UpdateGemUI();
}

ARespawnStashManager::ARespawnStashManager()
{
	PlayerInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("PlayerInventory"));
	StashInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("StashInventory"));
}

void ARespawnStashManager::BeginPlay()
{
	Super::BeginPlay();
	
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	if(GameInstance)
	{
		for(auto Item : GameInstance->PlayerSessionData.PlayerItems)
		{
			PlayerInventory->TryAddItemByAddItemInfo(Item.Value);
		}

		for(auto Item : GameInstance->PlayerSessionData.StashItems)
		{
			StashInventory->TryAddItemByAddItemInfo(Item.Value);
		}
	}
}

void ARespawnStashManager::AddGem(UItemObject* Gem, EBodyPart BodyPart)
{
	if(GemInventory.Contains(BodyPart))
	{
		return;
	}
	
	GemInventory.Add(BodyPart, Gem);
	UpdateGemUI();
}

void ARespawnStashManager::RemoveGem(EBodyPart BodyPart)
{
	if(!GemInventory.Contains(BodyPart))
	{
		return;
	}
	
	GemInventory.Remove(BodyPart);
	UpdateGemUI();
}

void ARespawnStashManager::Save()
 {
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	
	GameInstance->BuildPlayerSessionData(PlayerInventory->GetPlayerInventory(),
		StashInventory->GetPlayerInventory() , GetGemInventory());

}

TArray<FGemItem> ARespawnStashManager::GetGemInventoryStruct()
{
	TArray<FGemItem> GemItems;

	for(auto Item : GemInventory)
	{
		FGemItem GemItem;
		GemItem.Item = Item.Value;
		GemItem.BodyPart = Item.Key;
		GemItems.Add(GemItem);
	}

	return GemItems;
}

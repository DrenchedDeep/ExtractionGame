


#include "Core/Other/PlayerSaveData.h"

UPlayerSaveData::UPlayerSaveData(): bHasSeenIntroCutscene(false), bTutorialCrashLand(false), bTutorialInventory(false),
                                    bTutorialExtraction(false),
                                    BTutorialDeath(false)
{
}

void UPlayerSaveData::SetSeenCutscene(bool state)
{
	if(bHasSeenIntroCutscene == state) return;
	bHasSeenIntroCutscene = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetTutorialCrashLand(bool state)
{
	if(bTutorialCrashLand == state) return;
	bTutorialCrashLand = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetTutorialInventory(bool state)
{
	if(bTutorialInventory == state) return;
	bTutorialInventory = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetTutorialExtraction(bool state)
{
	if(bTutorialExtraction == state) return;
	bTutorialExtraction = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))
}

void UPlayerSaveData::SetTutorialDeath(bool state)
{
	if(BTutorialDeath == state) return;
	BTutorialDeath = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetStashInventory(TMap<int32, FAddItemInfo> InInventory)
{
	for(auto Item : InInventory)
	{
		FSavedInventoryItem NewItem;
		NewItem.ItemInfo = Item.Value;
		NewItem.Index = Item.Key;
		StashItems.Add(NewItem);
	}

	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
}

void UPlayerSaveData::SetPlayerInventory(TMap<int32, FAddItemInfo> InInventory)
{
	for(auto Item : InInventory)
	{
		FSavedInventoryItem NewItem;
		NewItem.ItemInfo = Item.Value;
		NewItem.Index = Item.Key;
		PlayerItems.Add(NewItem);
	}

	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
}

void UPlayerSaveData::SetGemInventory(TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> InInventory)
{
	for(auto Item : InInventory)
	{
		FSavedInventoryItem NewItem;
		NewItem.ItemInfo = Item.Value;
		NewItem.Index = Item.Key;
		GemItems.Add(NewItem);
	}

	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
}


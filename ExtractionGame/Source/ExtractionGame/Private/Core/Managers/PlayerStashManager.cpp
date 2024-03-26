#include "Core/Managers/PlayerStashManager.h"

#include "Components/ItemObject.h"
#include "UI/MainMenuHUD.h"
#include "Core/Other/PlayerSaveData.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Objects/ReplicatedItemObject.h"
#include "EntitySystem/MovieSceneEntityBuilder.h"


APlayerStashManager::APlayerStashManager()
{
	PlayerInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("PlayerInventory"));
	StashInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("StashInventory"));
}

void APlayerStashManager::BeginPlay()
{
	//check if first time launch by getting files check if there is a playersavedata
	Super::BeginPlay();
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	if(GameInstance->PlayerSessionData.bIsValid)
	{
		for(auto Item : GameInstance->PlayerSessionData.PlayerItems)
		{
			PlayerInventory->TryAddItemByAddItemInfo(Item.Value);
		}

		for(auto Item : GameInstance->PlayerSessionData.StashItems)
		{
			StashInventory->TryAddItemByAddItemInfo(Item.Value);
		}

		for(auto Gem : GameInstance->PlayerSessionData.GemItems)
		{
			if(UReplicatedItemObject* ItemObject = NewObject<UReplicatedItemObject>(	this,ItemObjectSubclass))
			{
				ItemObject->ItemName = Gem.Value.ItemName;
				ItemObject->ItemType =  Gem.Value.ItemType;
				ItemObject->Rarity =  Gem.Value.Rarity;
				ItemObject->Description =  Gem.Value.Description;
				ItemObject->GemType =  Gem.Value.GemType;
				ItemObject->DefaultPolish =  Gem.Value.DefaultPolish;
				ItemObject->Dimensions =  Gem.Value.Dimensions;
				ItemObject->Icon =  Gem.Value.Icon;
				ItemObject->IconRotated =  Gem.Value.IconRotated;
				ItemObject->RowName =  Gem.Value.RowName;
				AddGem(ItemObject, Gem.Key);
			}
		}

		InitGemUI();
	}
	else
	{
		GameInstance->ReadPlayerData("PlayerSaveData");
	}
	
	//GameInstance->GetFileCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnReadInventory);
//	GameInstance->UserReadCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnFilesRead);
	//GameInstance->UserWriteCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnSavedInventory);

	//LoadInventory();

}




void APlayerStashManager::SaveInventory()
{
	//disable for now
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	
	GameInstance->BuildPlayerSessionData(PlayerInventory->GetPlayerInventory(),
		StashInventory->GetPlayerInventory() , GetGemInventory());

	OnSave();
	UE_LOG(LogTemp, Warning, TEXT("Saving Inventory"));
	bool bDisable = true;

	if(bDisable)
	{
		return;
	}
	const FString FileName = TEXT("PlayerSaveData");

	bool bHasSavedSlot = UGameplayStatics::DoesSaveGameExist(FileName, 0);

	if(bHasSavedSlot)
	{
		UGameplayStatics::DeleteGameInSlot(FileName, 0);
	}

	UPlayerSaveData* PlayerSavedData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(PlayerSavedDataSubclass));

	//UE_LOG(LogTemp, Warning, TEXT("Stash Length: %i"), StashInventory->StashItems.Num());
	//UE_LOG(LogTemp, Warning, TEXT("Player Length: %i"), PlayerInventory->StashItems.Num());
	/*/
	for(int i = 0; i < StashInventory->GetItemsAsAddItemInfo().Num(); i++)
	{
		const FAddItemInfo InventoryItem = StashInventory->GetItemsAsAddItemInfo()[i];
		
		const uint32 Value = (static_cast<uint32>(InventoryItem.SlotID) << 16)
		| (static_cast<uint32>(InventoryItem.ItemID->ItemID) << 8) | InventoryItem.StackSize;

		FSavedInventoryItem SavedInventoryItem(Value);
		PlayerSavedData->StashItems.Add(SavedInventoryItem);
	}

	for(int i = 0; i < PlayerInventory->StashItems.Num(); i++)
	{
		const FInventoryItem InventoryItem = PlayerInventory->StashItems[i];

		if(InventoryItem.ItemID == nullptr)
		{
			continue;
		}
		
		const uint32 Value = (static_cast<uint32>(InventoryItem.SlotID) << 16)
		| (static_cast<uint32>(InventoryItem.ItemID->ItemID) << 8) | InventoryItem.StackSize;

		FSavedInventoryItem SavedInventoryItem(Value);
		PlayerSavedData->PlayerItems.Add(SavedInventoryItem);
	}

	UGameplayStatics::SaveGameToSlot(PlayerSavedData, FileName, 0);

	TArray<uint8> SavedData = GameInstance->ConvertSavedFileToInt(PlayerSavedData);
	GameInstance->UpdatePlayerData(FileName, SavedData);
	/*/
}

void APlayerStashManager::AddGem(UItemObject* Gem, EBodyPart BodyPart)
{
	if(GemInventory.Contains(BodyPart))
	{
		return;
	}
	
	GemInventory.Add(BodyPart, Gem);
}

void APlayerStashManager::RemoveGem(EBodyPart BodyPart)
{
	if(!GemInventory.Contains(BodyPart))
	{
		return;
	}
	
	GemInventory.Remove(BodyPart);
}


TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> APlayerStashManager::GetGemInventory()
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

		GemItems.Add(Item.Key, Info);
	}

	return GemItems;
}

TArray<FGemItem> APlayerStashManager::GetGemInventoryStruct()
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

/*/
void APlayerStashManager::LoadInventory()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	GameInstance->ReadPlayerData("PlayerSaveData");
}


void APlayerStashManager::OnReadInventory(FString FileName, TArray<uint8> DataRef)
{
	
	if(FileName != "PlayerSaveData")
	{
		return;
	}
	
	bool bHasSavedSlot = UGameplayStatics::DoesSaveGameExist(FileName, 0);

	if(bHasSavedSlot)
	{
		UGameplayStatics::DeleteGameInSlot(FileName, 0);
	}
	
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	USaveGame* SaveGame = GameInstance->ConvertIntToSavedFile(DataRef);

	if(const UPlayerSaveData* PlayerSavedData = Cast<UPlayerSaveData>(SaveGame))
	{
		for(int i = 0; i < PlayerSavedData->StashItems.Num(); i++)
		{
			const uint8 SlotID = static_cast<uint8>((PlayerSavedData->StashItems[i].InventoryData >> 16) & 0xFF);
			const uint8 ItemID = static_cast<uint8>((PlayerSavedData->StashItems[i].InventoryData >> 8) & 0xFF);
			const uint8 StackSize = static_cast<uint8>(PlayerSavedData->StashItems[i].InventoryData & 0xFF);

			UItem* Item = ItemDatabase->FindItem(ItemID);
			StashInventory->AddItem(Item, StackSize, true, SlotID);
		}

		if(!GameInstance->bLoadedInventoryOnStart)
		{
			for(int i = 0; i < PlayerSavedData->PlayerItems.Num(); i++)
			{
				const uint8 SlotID = static_cast<uint8>((PlayerSavedData->PlayerItems[i].InventoryData >> 16) & 0xFF);
				const uint8 ItemID = static_cast<uint8>((PlayerSavedData->PlayerItems[i].InventoryData >> 8) & 0xFF);
				const uint8 StackSize = static_cast<uint8>(PlayerSavedData->PlayerItems[i].InventoryData & 0xFF);

				UItem* Item = ItemDatabase->FindItem(ItemID);
				PlayerInventory->AddItem(Item, StackSize, true, SlotID);
			}
			GameInstance->bLoadedInventoryOnStart = true;
		}
		const TArray<FName> PartyMembers;
		GameInstance->BuildPlayerSessionData(PlayerInventory->StashItems, PartyMembers);
		SaveInventory();
	}
}


void APlayerStashManager::OnSavedInventory(bool bWasSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Saved Inventory"));
}

void APlayerStashManager::OnFilesRead(bool bWasSuccess)
{
		bRanFirstTimeCheck = true;
}

/*/




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
	
	//GameInstance->GetFileCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnReadInventory);
//	GameInstance->UserReadCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnFilesRead);
	//GameInstance->UserWriteCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnSavedInventory);

	//LoadInventory();

	if(!GameInstance->SaveData)
	{
		return;
	}
	if(!GameInstance->SaveData->GetLoadInventory())
	{
		InitStartingItems();
		GameInstance->TotalExtractedEssence = 75;
		GameInstance->SaveData->SetLoadInventory(true);
		GameInstance->SaveData->SetExtractedEssence(GameInstance->TotalExtractedEssence);

	}
	else
	{
		if(!GameInstance->bHasLoadedSaveInventory)
		{
		GameInstance->TotalExtractedEssence = GameInstance->SaveData->GetExtractedEssence();
			
		for(auto Item : GameInstance->SaveData->PlayerItems)
		{
			PlayerInventory->TryAddItemByAddItemInfo(Item.ItemInfo);
		}

		for(auto Item :GameInstance->SaveData->StashItems)
		{
			StashInventory->TryAddItemByAddItemInfo(Item.ItemInfo);
		}

	
			for(auto Gem : GameInstance->SaveData->GemItems)
			{
				if(UReplicatedItemObject* ItemObject = NewObject<UReplicatedItemObject>(	this,ItemObjectSubclass))
				{
					ItemObject->ItemName = Gem.ItemInfo.ItemName;
					ItemObject->ItemType =  Gem.ItemInfo.ItemType;
					ItemObject->Rarity =  Gem.ItemInfo.Rarity;
					ItemObject->Description =  Gem.ItemInfo.Description;
					ItemObject->GemType =  Gem.ItemInfo.GemType;
					ItemObject->DefaultPolish =  Gem.ItemInfo.DefaultPolish;
					ItemObject->Dimensions =  Gem.ItemInfo.Dimensions;
					ItemObject->Icon =  Gem.ItemInfo.Icon;
					ItemObject->IconRotated =  Gem.ItemInfo.IconRotated;
					ItemObject->RowName =  Gem.ItemInfo.RowName;
					AddGem(ItemObject, static_cast<EBodyPart>(Gem.Index));
				}
			}

			GameInstance->bHasLoadedSaveInventory = true;
		}
	}
}




void APlayerStashManager::SaveInventory()
{
	//disable for nowz
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	
	GameInstance->BuildPlayerSessionData(PlayerInventory->GetPlayerInventory(),
		StashInventory->GetPlayerInventory() , GetGemInventory());

	OnSave();
	
	GameInstance->SaveData->SetGemInventory(GetGemInventory());
	GameInstance->SaveData->SetPlayerInventory(PlayerInventory->GetPlayerInventory());
	GameInstance->SaveData->SetStashInventory(StashInventory->GetPlayerInventory());
}

void APlayerStashManager::AddGem(UItemObject* Gem, EBodyPart BodyPart)
{
	if(GemInventory.Contains(BodyPart))
	{
		return;
	}
	
	GemInventory.Add(BodyPart, Gem);
	OnGemInventoryChanged();
}

void APlayerStashManager::RemoveGem(EBodyPart BodyPart)
{
	if(!GemInventory.Contains(BodyPart))
	{
		return;
	}
	
	GemInventory.Remove(BodyPart);
	OnGemInventoryChanged();
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
		Info.RowName = Item.Value->RowName;

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
		GemItem.Polish = Item.Value->DefaultPolish;
		GemItems.Add(GemItem);
	}

	return GemItems;
}

FAbilityStruct APlayerStashManager::GetLeftArmAbilityInfo()
{
	TArray<UItemObject*> LeftArmGems;

	for(auto Gem : GemInventory)
	{
		if(Gem.Key >= EBodyPart::LeftArm0 && Gem.Key <= EBodyPart::LeftArm1)
		{
			LeftArmGems.Add(Gem.Value);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("length: %d"), LeftArmGems.Num());

	float type [] = {0,0,0,0};
	for(auto Gem : LeftArmGems)
	{
		if(!Gem)
		{
			continue;
		}

		switch(Gem->GemType)
		{
		case EGemType::Earth:
			type [0]+=Gem->DefaultPolish;
			break;
		case EGemType::Fire:
			type [1]+=Gem->DefaultPolish;
			break;
		case EGemType::Shadow:
			type [2]+=Gem->DefaultPolish;
			break;
		case EGemType::Water:
			type [3]+=Gem->DefaultPolish;
			break;
		default:  ;
		}
	}

	int32 ability = 0;
	int iteration= 0;
	float totalPolish = 0;
	for (const float val : type)
	{
		totalPolish += val;
		int Score;
		if(val >= 150) Score = 3;
		else if(val >= 75) Score = 2;
		else if (val > 0) Score = 1;
		else Score = 0;
		ability |= Score << (8-(++iteration*2));
	}

	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	const FAbilityStruct InAbilityClass = GameInstance->GetAbilityHandlerSubSystem()->GetAbilityByIndex(ability);

	return InAbilityClass;
}



FAbilityStruct APlayerStashManager::GetRightArmAbilityInfo()
{
	TArray<UItemObject*> RightArmGems;

	for(auto Gem : GemInventory)
	{
		if(Gem.Key >= EBodyPart::RightArm0 && Gem.Key <= EBodyPart::RightArm1)
		{
			RightArmGems.Add(Gem.Value);
		}
	}

	float type [] = {0,0,0,0};
	for(auto Gem : RightArmGems)
	{
		if(!Gem)
		{
			continue;;
		}

		switch(Gem->GemType)
		{
		case EGemType::Earth:
			type [0]+=Gem->DefaultPolish;
			break;
		case EGemType::Fire:
			type [1]+=Gem->DefaultPolish;
			break;
		case EGemType::Shadow:
			type [2]+=Gem->DefaultPolish;
			break;
		case EGemType::Water:
			type [3]+=Gem->DefaultPolish;
			break;
		default:  ;
		}
	}

	int32 ability = 0;
	int iteration= 0;
	float totalPolish = 0;
	for (const float val : type)
	{
		totalPolish += val;
		int Score;
		if(val >= 150) Score = 3;
		else if(val >= 75) Score = 2;
		else if (val > 0) Score = 1;
		else Score = 0;
		ability |= Score << (8-(++iteration*2));
	}

	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	const FAbilityStruct InAbilityClass = GameInstance->GetAbilityHandlerSubSystem()->GetAbilityByIndex(ability);

	return InAbilityClass;
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







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


FAbilityStruct ARespawnStashManager::GetLeftArmAbilityInfo()
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



FAbilityStruct ARespawnStashManager::GetRightArmAbilityInfo()
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

TArray<FGemItem> ARespawnStashManager::GetGemInventoryStruct()
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

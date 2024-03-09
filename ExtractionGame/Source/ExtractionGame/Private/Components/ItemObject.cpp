


#include "Components/ItemObject.h"
#include "Components/ItemObject.h"
#include "Components/ItemObject.h"

#include "Editor.h"
#include "ExtractionGameBPLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Components/InventoryComp.h"

void UItemObject::Rotate()
{
	bRotated = !bRotated;
}

void UItemObject::Init(FVector2D Size, UMaterialInterface* InIcon, UMaterialInterface* InIconRotated, FString InName,
	EItemTypes InType, ERarityType InRarity, FString Desc, EGemType InGemType, float InPolish, FName InRowName,
	int32 InItemCost)
{
	Dimensions = Size;
	this->Icon = InIcon;
	this->IconRotated = InIconRotated;
	ItemName = InName;
	ItemType = InType;
	this->Rarity = InRarity;
	Description = Desc;
	this->GemType = InGemType;
	DefaultPolish = InPolish;
	FString T = FString::SanitizeFloat(DefaultPolish);
	UE_LOG(LogTemp, Warning, TEXT("Default Polish: %s"), *T);
	RowName = InRowName;
	ItemCost = InItemCost;


	if(InType == EItemTypes::Gem)
	{
		const ERarityType NewRarity = UExtractionGameBPLibrary::GetRarityTypeFromGemPurity(InPolish);
		SetRarity(NewRarity);

		ItemCost = UExtractionGameBPLibrary::GetAdjustedPrice(NewRarity, ItemCost, InPolish);
	}
}


void UItemObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, Dimensions);
	DOREPLIFETIME(UItemObject, Icon);
	DOREPLIFETIME(UItemObject, IconRotated);
	DOREPLIFETIME(UItemObject, ItemName);
	DOREPLIFETIME(UItemObject, ItemType);
	DOREPLIFETIME(UItemObject, Rarity);
	DOREPLIFETIME(UItemObject, Description);
	DOREPLIFETIME(UItemObject, GemType);
	DOREPLIFETIME(UItemObject, DefaultPolish);
	DOREPLIFETIME(UItemObject, RowName);
	DOREPLIFETIME(UItemObject, ItemCost);
}

void UItemObject::BeginDestroy()
{
	UObject::BeginDestroy();

	UE_LOG(LogTemp, Warning, TEXT("ItemObject Destroyed"));
}

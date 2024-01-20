


#include "Components/PlayerInventoryComponent.h"

#include "Components/ItemObject.h"
#include "UI/ExtractionGameHUD.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Net/UnrealNetwork.h"


void UPlayerInventoryComponent::InitStartingItems()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());

	if(GameInstance)
	{
		for(auto Item : GameInstance->PlayerSessionData.PlayerItems)
		{
			UE_LOG(LogTemp, Warning, TEXT("Items!!!!!!!"));
			Server_AddItem(Item.Value, Item.Key);
			//	Character->GemController->Server_AddGem()
		}
	}
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
		if(!GemItems[i])
		{
			continue;
		}

		if(Itms.Contains(GemItems[i]))
		{
			continue;
		}

		Itms.Add(GemItems[i]);
	}	

	return Itms;
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPlayerInventoryComponent, GemItems, COND_OwnerOnly);
}

void UPlayerInventoryComponent::Server_AddGem_Implementation(UItemObject* Item)
{
	GemItems.Add(Item);
}

void UPlayerInventoryComponent::Server_RemoveGem_Implementation(UItemObject* Item)
{
	GemItems.Remove(Item);
}

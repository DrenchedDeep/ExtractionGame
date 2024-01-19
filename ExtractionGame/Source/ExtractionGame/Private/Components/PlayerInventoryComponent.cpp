


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
			Server_AddItem(Item.Value, Item.Key);
			//	Character->GemController->Server_AddGem()
		}
	}
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

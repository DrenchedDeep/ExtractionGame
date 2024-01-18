


#include "Components/PlayerInventoryComponent.h"

#include "Components/ItemObject.h"
#include "UI/ExtractionGameHUD.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"


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

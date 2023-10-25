


#include "PlayerInventoryComponent.h"

#include "ExtractionGameHUD.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "ExtractionGame/ExtractionGameInstance.h"

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AExtractionGameCharacter>(GetOwner());

	if(Character->IsLocallyControlled())
	{
		if(AExtractionGameHUD* GameHUD = Cast<AExtractionGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			if(GameHUD->InventoryWidget)
			{
				InventoryWidget = GameHUD->InventoryWidget;
				return;
			}
			
			InventoryWidget = GameHUD->CreateInventoryWidget();
			InventoryWidget->Init(this, 20);

			if(InventoryWidget)
			{

			}
		}
	}

	if(Character->HasAuthority() || Character->IsLocallyControlled())
	{
		if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance()))
		{
			TArray<FInventoryItem> PlayerItems = GameInstance->PlayerSessionData.PlayerItems;
			UE_LOG(LogTemp, Warning, TEXT("%i"), PlayerItems.Num());

			for(int i = 0; i < PlayerItems.Num(); i++)
			{
				AddItem(PlayerItems[i].ItemID, PlayerItems[i].StackSize, true, PlayerItems[i].SlotID);

				if(PlayerItems[i].ItemID->ItemType == Gem && Character->HasAuthority())
				{
					Character->GemController->Server_CreateGem(PlayerItems[i].ItemID, static_cast<EBodyPart>(PlayerItems[i].GemSlotID), PlayerItems[i].SlotID);
				}
			}
		}
		
	}
}

void UPlayerInventoryComponent::OnRep_InventoryItems()
{
	if(!Character)
	{
		return;
	}

	if(Character->IsLocallyControlled() && bReconcileVisuals)
	{
		for(int i = 0; i < InventoryItems.Num(); i++)
		{
			TArray<USlotWidget*> Slots = InventoryWidget->GetSlots();
			
			for(int z = 0; z < Slots.Num(); z++)
			{
				if(InventoryItems[i].SlotID == Slots[z]->GetSlotID())
				{
					Slots[z]->ReconcileVisuals(InventoryItems[i]);
				}
			}
		}
	}
}

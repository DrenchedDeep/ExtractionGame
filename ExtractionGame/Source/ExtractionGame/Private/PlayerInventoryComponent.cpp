


#include "PlayerInventoryComponent.h"

#include "ExtractionGameHUD.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "ExtractionGame/ExtractionGameInstance.h"

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

//	InitInventory();
}

void UPlayerInventoryComponent::InitInventory()
{
	Character = Cast<AExtractionGameCharacter>(GetOwner());

	if(Character->IsLocallyControlled())
	{
		if(AExtractionGameHUD* GameHUD = Cast<AExtractionGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			InventoryWidget = GameHUD->CreateInventoryWidget();
			InventoryWidget->Init(this, 20);

			if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance()))
			{
				TArray<FInventoryItem> PlayerItems = GameInstance->PlayerSessionData.PlayerItems;

				if(PlayerItems.Num() > 0)
				{
					for(int i = 0; i < PlayerItems.Num(); i++)
					{
						AddItem(PlayerItems[i].ItemID, PlayerItems[i].StackSize, true, PlayerItems[i].SlotID);
					}
				}
			}
		}
	}
}

void UPlayerInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
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

void UPlayerInventoryComponent::AddItem(UItem* Item, int StackSize, bool bClientSimulation, int SlotID)
{
	Super::AddItem(Item, StackSize, bClientSimulation, SlotID);
	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UPlayerInventoryComponent::InitGemDelay, Item, SlotID);

	GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, 1.0f, false);
}

void UPlayerInventoryComponent::InitGemDelay(UItem* Item, int SlotID)
{
	if(Item->ItemType == EItemType::Gem)
	{
		if(UGemSlot* GemSlot = Cast<UGemSlot>(InventoryWidget->GetSlot(SlotID)))
		{
			Character->GemController->Server_CreateGem(Item, GemSlot->BodyPart, SlotID);
		}
	}
}

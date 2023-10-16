


#include "InventoryComponent.h"

#include "ExtractionGameHUD.h"
#include "IDetailTreeNode.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "Net/UnrealNetwork.h"


void UInventoryComponent::BeginPlay()
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
				for(int i = 0; i < StartingItems.Num(); i++)
				{
					AddItem(StartingItems[i].ItemID, StartingItems[i].Stack, true);
				}
			}
		}
	}

	if(Character->HasAuthority())
	{
		for(int i = 0; i < StartingItems.Num(); i++)
		{
			AddItem(StartingItems[i].ItemID, StartingItems[i].Stack, false);
		}
	}
}

void UInventoryComponent::OnRep_InventoryItems()
{
	if(Character->IsLocallyControlled() && bReconcileVisuals)
	{
		for(int i = 0; i < InventoryItems.Num(); i++)
		{
			TArray<USlotWidget*> Slots = InventoryWidget->GetSlots();
			
			for(int z = 0; z < Slots.Num(); i++)
			{
				if(InventoryItems[i].SlotID == Slots[z]->GetSlotID())
				{
					Slots[z]->ReconcileVisuals(InventoryItems[i]);
				}
			}
		}
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryItems);
}

void UInventoryComponent::Server_TransferSlots_Implementation(int TargetInventoryItemID, int NewSlotID)
{
	for(int i = 0; i < InventoryItems.Num(); i++)
	{
		if(TargetInventoryItemID == InventoryItems[i].InventoryID)
		{
			InventoryItems[i].SlotID = NewSlotID;
			break;
		}
	}
}


void UInventoryComponent::AddItem(UItem* Item, int StackSize, bool bClientSimulation)
{
	const int InventoryIndex = FMath::RandRange(0, 10000);

	//if were a client, predict adding the item (just do the visuals)
	//and rpc to the server to actually add the item
	//if the server had a problem adding an item, we can reconcile visuals in a client rpc
	if(bClientSimulation)
	{
		if(USlotWidget* SlotWidget = InventoryWidget->GetNextSlot())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Next Slot"));

			SlotWidget->PredictVisuals(Item, StackSize);

			//need to set the inventory index on the local client aswell
			SlotWidget->SetInventoryIndex(InventoryIndex);
			Server_AddItem(Item, StackSize, InventoryIndex, SlotWidget->GetSlotID());
		}
	}

	if(GetOwner()->HasAuthority())
	{
		//if were the server, add the item directly
	}
}

void UInventoryComponent::RemoveItem(UItem* Item, int StackSize, bool bClientSimulation)
{
	
}

void UInventoryComponent::TransferSlots(USlotWidget* OldSlot, USlotWidget* NewSlot)
{
	//if the new slot already has an item, we want to swap the slots items
	if(NewSlot->bIsOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("New Slot already has item"));
		return;
	}

	//update inventory
	Server_TransferSlots(OldSlot->GetInventoryIndex(), NewSlot->GetSlotID());


	//update visuals
	NewSlot->PredictVisuals(OldSlot->GetCurrentItem(),  OldSlot->GetCurrentStack());
	NewSlot->SetInventoryIndex(OldSlot->GetInventoryIndex());
	OldSlot->Reset();
}


void UInventoryComponent::Server_RemoveItem_Implementation(UItem* Item, int StackSize)
{
}


bool UInventoryComponent::Server_RemoveItem_Validate(UItem* Item, int StackSize)
{
	return true;
}


void UInventoryComponent::Server_AddItem_Implementation(UItem* Item, int StackSize, int InventoryIndex,int SlotID)
{
	//if were the server, add the item directly
	InventoryItems.Add(FInventoryItem(InventoryIndex,SlotID, StackSize, Item));
}

bool UInventoryComponent::Server_AddItem_Validate(UItem* Item, int StackSize, int InventoryIndex, int SlotID)
{
	//in here we can check if the player is close to a chest and if it has said item to prevent cheating, stuff like that
	return true;
}


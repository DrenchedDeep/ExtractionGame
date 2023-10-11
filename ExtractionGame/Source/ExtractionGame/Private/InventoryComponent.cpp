


#include "InventoryComponent.h"

#include "ExtractionGameHUD.h"
#include "IDetailTreeNode.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "Net/UnrealNetwork.h"



void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

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
			InventoryWidget->Init(20);

			if(InventoryWidget)
			{
				for(int i = 0; i < StartingItems.Num(); i++)
				{
					AddItem(StartingItems[i].ItemID, StartingItems[i].Stack);
				}
			}
		}
	}
}

//we'll do all the adding items client side 
void UInventoryComponent::AddItem(UItem* ItemData, int Stack)
{
	const FInventoryItem TempInventoryItem = FindItem(ItemData);
	if(TempInventoryItem.ItemID)
	{
		InventoryItems[TempInventoryItem.SlotID].StackSize += Stack;
	}
	else
	{
		InventoryItems.Add(FInventoryItem(InventoryItems.Num(), Stack, ItemData));
	}


	USlotWidget* SlotWidget = InventoryWidget->GetNextSlot();
	const FInventoryItem InventoryItem(true, Stack, ItemData);
	if(SlotWidget)
	{
		InventoryWidget->SetSlot(SlotWidget->GetSlotID(), InventoryItem);
		//need to do thus on server aswell
		InventoryItems.Add(InventoryItem);
	}
}

void UInventoryComponent::RemoveItem(UItem* ItemData, int Stack)
{
}

void UInventoryComponent::RemoveItem(int SlotID, int Stack)
{
}

FInventoryItem UInventoryComponent::FindItem(UItem* Item)
{
	FInventoryItem InventoryItem = {};
	for(int i = 0; i < InventoryItems.Num(); i++)
	{
		if(Item == InventoryItems[i].ItemID)
		{
			InventoryItem = InventoryItems[i];
		}
	}
	
	return InventoryItem;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryItems);
}

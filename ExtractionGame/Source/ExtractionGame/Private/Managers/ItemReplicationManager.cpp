


#include "Managers/ItemReplicationManager.h"

#include "Items/ItemActor.h"
#include "Net/UnrealNetwork.h"

/*/

FItemActorItem::FItemActorItem()
{
	
}

FItemActorItem::FItemActorItem(AActor* InOwnerActor, const TArray<uint8>& InData)
{
	OwnerActor = InOwnerActor;
	Data = InData;
}

void FItemActorItem::PostReplicatedChange(const FItemActorContainer& InArraySerializer)
{
	if(OwnerActor.Get())
	{
		if(AItemActor* ItemActor = Cast<AItemActor>(OwnerActor.Get()))
		{
			ItemActor->PostReplication(Data);
		}
	}
}

// Sets default values
AItemReplicationManager::AItemReplicationManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AItemReplicationManager::RegisterActor(AActor* Actor, const TArray<uint8>& Data)
{
	if(Actor)
	{
		if(AItemActor* ItemActor = Cast<AItemActor>(Actor))
		{
			const FItemActorItem Item(ItemActor, Data);
			ItemActors.MarkItemDirty(ItemActors.Items.Add_GetRef(Item));
		}
	}
}

void AItemReplicationManager::UpdateActor(AActor* Actor, const TArray<uint8>& Data)
{
	FItemActorItem* FoundEntry = ItemActors.Items.FindByPredicate([Actor](const FItemActorItem& Item)
	{
		return Item.OwnerActor.Get() == Actor;
	});

	if(FoundEntry)
	{
		FoundEntry->Data = Data;
		ItemActors.MarkItemDirty(*FoundEntry);
	}
}

void AItemReplicationManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemReplicationManager, ItemActors);
}

FItemActorItem::FItemActorItem()
{
	
}

FItemActorItem::FItemActorItem(AActor* InOwnerActor, const TArray<uint8>& InData)
{
	OwnerActor = InOwnerActor;
	Data = InData;
}


void FItemActorItem::PostReplicatedChange(const struct FItemActorContainer& InArraySerializer)
{
	if(OwnerActor.Get())
	{
		if(AItemActor* ItemActor = Cast<AItemActor>(OwnerActor.Get()))
		{
			ItemActor->PostReplication(Data);
		}
	}
}
/*/
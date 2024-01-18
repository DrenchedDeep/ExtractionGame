


#include "Managers/ItemReplicationManager.h"

#include "IDetailTreeNode.h"
#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemSpawner.h"
#include "Net/UnrealNetwork.h"


void UItemReplicationManager::Init()
{
	UWorld* World = GetWorld();
	if(!World)
	{
		World = GetOuter()->GetWorld();
	}
	
	for(FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if(AExtractionGamePlayerController* PlayerController = Cast<AExtractionGamePlayerController>(Iterator->Get()))
		{
			PlayerController->Client_SpawnItems();
		}
	}
}

int32 UItemReplicationManager::RegisterNewItem(AItemActor* ItemActor)
{
	ItemPickups.Add(	{ItemActor, false}	);

	return ItemPickups.Num() - 1;
}

void UItemReplicationManager::UpdateItemState(int32 Index, bool bIsPickedUp)
{
	ItemPickups[Index].bIsPickedUp = bIsPickedUp;

	if(bIsPickedUp)
	{
		Multicast_UpdateItem(ItemPickups[Index].OwnerActor);
	}
}

void UItemReplicationManager::UnregisterItem(int32 Index)
{
	Server_UpdateItemState(Index, true);
}

void UItemReplicationManager::Multicast_UpdateItem_Implementation(AItemActor* ItemActor)
{
//	ItemActor->Destroy();
}


void UItemReplicationManager::SpawnItems()
{
	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client spawning items"));
	}
	TArray<AActor*> ItemSpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawner::StaticClass(), ItemSpawnPoints);


	for (auto ItemSpawnPoint : ItemSpawnPoints)
	{
		AItemSpawner* ItemSpawner = Cast<AItemSpawner>(ItemSpawnPoint);
		if (ItemSpawner)
		{
			ItemSpawner->SpawnItems(this);
		}
	}
}

void UItemReplicationManager::Server_UpdateItemState_Implementation(int32 Index, bool bIsPickedUp)
{
	UpdateItemState(Index, bIsPickedUp);
}

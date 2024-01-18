


#include "Items/ItemActor.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/ItemSpawner.h"

void AItemActor::UnregisterItem()
{
//	ItemSpawner->GetItemReplicationManager()->UnregisterItem(Index);
	Destroy();
}

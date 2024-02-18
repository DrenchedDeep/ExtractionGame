


#include "Managers/ItemReplicationManager.h"

#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemSpawner.h"
#include "Net/UnrealNetwork.h"

void AItemReplicationManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> ItemSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawner::StaticClass(),ItemSpawners);

	for(AActor* ItemSpawner : ItemSpawners)
	{
		if(AItemSpawner* Spawner = Cast<AItemSpawner>(ItemSpawner))
		{
			Spawner->SpawnItems(this);
		}
	}
}




#include "AI/GooberManager.h"
#include "Kismet/GameplayStatics.h"


TArray<AGoober*> AGooberManager::GetAllGoobers()
{
	TArray<AGoober*> OutGoobers;

	for(const auto SpawnCluster : SpawnClusters)
	{
		for(auto Goober : SpawnCluster->GetAllGoobers())
		{
			OutGoobers.Add(Goober);
		}
	}

	return OutGoobers;
}

void AGooberManager::AddToRespawnStream(AGooberSpawnCluster* Cluster)
{
	if(bOnlyAllowSameClusterInStreamOnce && SpawnBuffer.Contains(Cluster))
	{
		return;
	}
	
	SpawnBuffer.Add(Cluster);
}

void AGooberManager::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGooberSpawnCluster::StaticClass(), FoundActors);

		if(FoundActors.Num() > 0)
		{
			for(const auto Actor : FoundActors)
			{
				if(AGooberSpawnCluster* SpawnCluster = Cast<AGooberSpawnCluster>(Actor))
				{
					SpawnClusters.Add(SpawnCluster);
				}
			}
		}

		
		for(auto SpawnCluster : SpawnClusters)
		{
			SpawnCluster->SpawnGoobers(this);
		}

		GetWorld()->GetTimerManager().SetTimer(GooberRespawnHandle,
			this, &AGooberManager::TickGooberRespawnHandle, RespawnRate, true);
	}
}

void AGooberManager::TickGooberRespawnHandle()
{
	if(SpawnBuffer.IsValidIndex(0))
	{
		SpawnBuffer[0]->SpawnGoober();
		SpawnBuffer.RemoveAt(0);
	}
}

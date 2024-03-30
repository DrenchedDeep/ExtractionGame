


#include "AI/GooberManager.h"
#include "Kismet/GameplayStatics.h"

TWeakObjectPtr<AGooberManager> AGooberManager::GooberInstance = nullptr;

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

void AGooberManager::Multicast_PlayExplosion_Implementation(FVector Location)
{
	PlayExplosionEffect(Location);
}


void AGooberManager::Multicast_PickupGem_Implementation(AActor* GemActor, AGoober* Goober)
{
	PlayPickupGem(GemActor, Goober);
}

void AGooberManager::Multicast_Attack_Implementation(AGoober* Goober)
{
	PlayAttackAnim(Goober);
}

void AGooberManager::Multicast_ResetGem_Implementation(AActor* Gem)
{
	ResetGemSize(Gem);
}

void AGooberManager::AddToRespawnStream(AGooberSpawnCluster* Cluster)
{
	if(bOnlyAllowSameClusterInStreamOnce && SpawnBuffer.Contains(Cluster))
	{
		return;
	}
	
	SpawnBuffer.Add(Cluster);
}

void AGooberManager::OnGeyserOpened(AGeyserSpawnpoint* Geyser)
{
	TArray<AGoober*> AllGoobers = GetAllGoobers();

	for(auto Goober : AllGoobers)
	{
		if(Goober && Goober->GetGooberState() != EGooberStates::EG_Friendly)
		{
			float Dist = (Goober->GetActorLocation() - Geyser->GetActorLocation()).Length();

			if(Dist <= GooberGeyserRange)
			{
				Goober->MoveToGeyser(Geyser);
			}
		}
	}
}

void AGooberManager::BeginPlay()
{
	Super::BeginPlay();

	GooberInstance = this;
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

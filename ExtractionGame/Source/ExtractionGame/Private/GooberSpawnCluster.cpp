


#include "GooberSpawnCluster.h"

#include "NavigationSystem.h"
#include "AI/GooberManager.h"

void AGooberSpawnCluster::SpawnGoobers(AGooberManager* InGooberManager)
{
	GooberManager = InGooberManager;
	for(int32 i = 0; i < MaxGoobersToSpawn; i++)
	{
		SpawnGoober();
	}
}

void AGooberSpawnCluster::UnRegisterGoober(AGoober* Goober)
{
	if(!AllGoobers.Contains(Goober))
	{
		return;
	}

	AllGoobers.Remove(Goober);

	if(bMakeSureGooberCountIsAtMax)
	{
		GooberManager->AddToRespawnStream(this);
	}
}

void AGooberSpawnCluster::SpawnGoober()
{
	if(!GooberManager)
	{
		return;
	}

	const int32 RandomSpawnPoint = FMath::RandRange(0, GooberSpawnPoints.Num()-1);

	if(GooberSpawnPoints.IsValidIndex(RandomSpawnPoint))
	{
		FVector SpawnLocation = GooberSpawnPoints[RandomSpawnPoint]->GetActorLocation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AGoober* Goober =
			Cast<AGoober>(GetWorld()->SpawnActor<AGoober>(GooberManager->GooberSubclass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters));

		if(Goober)
		{
			AllGoobers.Add(Goober);
			Goober->Init(this);
		}
	}
}

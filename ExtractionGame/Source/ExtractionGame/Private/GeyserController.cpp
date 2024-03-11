


#include "GeyserController.h"


void AGeyserController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::SpawnGeyser, GeyserSpawnDelay, true);
}

void AGeyserController::SpawnGeyser()
{
	if(GeyserSpawnpoints.IsValidIndex(LastIndex))
	{
		GeyserSpawnpoints[LastIndex]->DeactivateGeyser();
	}
	
	if(GeyserSpawnCount < GeyserMaxSpawns)
	{
		if(GeyserSpawnpoints.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, GeyserSpawnpoints.Num() - 1);
			if(GeyserSpawnpoints.IsValidIndex(RandomIndex) && !GeyserSpawnpoints[RandomIndex]->bGeyserActivated)
			{
				GeyserSpawnpoints[RandomIndex]->ActivateGeyser();
				LastIndex = RandomIndex;
			}
			
			GeyserSpawnCount++;
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GeyserSpawnTimer);
	}
}




#include "GeyserController.h"


void AGeyserController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::SpawnGeyser, GeyserSpawnDelay, true);
}

void AGeyserController::SpawnGeyser()
{
	if(GeyserSpawnCount < GeyserMaxSpawns)
	{
		if(GeyserSpawnpoints.IsValidIndex(LastIndex))
		{
			GeyserSpawnpoints[LastIndex]->DeactivateGeyser();
		}
		if(GeyserSpawnpoints.Num() > 0)
		{

			if(GeyserSpawnpoints.IsValidIndex(GeyserSpawnCount) && !GeyserSpawnpoints[GeyserSpawnCount]->bGeyserActivated)
			{
				GeyserSpawnpoints[GeyserSpawnCount]->ActivateGeyser();
				LastIndex = GeyserSpawnCount;
			}
			
			GeyserSpawnCount++;
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GeyserSpawnTimer);
	}
}

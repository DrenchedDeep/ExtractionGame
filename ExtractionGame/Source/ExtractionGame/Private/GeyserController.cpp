


#include "GeyserController.h"


void AGeyserController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::SpawnGeyser, GeyserSpawnDelay, true);
	SpawnGeyser();
}

void AGeyserController::SpawnGeyser()
{
	for(auto Geyser : GeyserSpawnpoints)
	{
		if(Geyser->GetGeyserState() != EGeyserStates::Deactive)
		{
			Geyser->ResetGeyser();
		}
	}

	int32 RandomChanceOfSpawning = FMath::RandRange(1, 2);

	if(RandomChanceOfSpawning == 1)
	{
	//	return;
	}

	int32 RandomGeyser = FMath::RandRange(0, GeyserSpawnpoints.Num() - 1);
	GeyserSpawnpoints[RandomGeyser]->SetGeyserState(EGeyserStates::Smoking);
}

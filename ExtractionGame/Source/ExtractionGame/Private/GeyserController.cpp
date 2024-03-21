


#include "GeyserController.h"


void AGeyserController::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		for(auto Geyser : GeyserSpawnpoints)
		{
			Geyser->SetGeyserState(EGeyserStates::Deactive);
		}
		GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::SpawnGeyser, GeyserSpawnDelay, true);
		SpawnGeyser();
	}
}

void AGeyserController::SpawnGeyser()
{
	if(ActiveGeyser)
	{
		ActiveGeyser->SetGeyserState(EGeyserStates::Deactive);
	}

	if(SmokingGeiser)
	{
		SmokingGeiser->SetGeyserState(EGeyserStates::Active);
		ActiveGeyser = SmokingGeiser;
	}

	int32 RandomIndex = FMath::RandRange(0, GeyserSpawnpoints.Num() - 1);
	
	if(GeyserSpawnpoints.IsValidIndex(RandomIndex))
	{
		SmokingGeiser = GeyserSpawnpoints[RandomIndex];
		SmokingGeiser->SetGeyserState(EGeyserStates::Smoking);
	}
}

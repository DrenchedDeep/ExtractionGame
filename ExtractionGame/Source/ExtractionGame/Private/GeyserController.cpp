


#include "GeyserController.h"


void AGeyserController::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		for(const auto Geyser : GeyserSpawnpoints)
		{
			Geyser->SetGeyserState(EGeyserStates::Deactive);
		}
		GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::NearlySpawnGeyser, GeyserSpawnDelay - GeyserNearlyReadyTime, true);
		//SpawnGeyser();
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

	
}

void AGeyserController::NearlySpawnGeyser()
{
	GetWorldTimerManager().ClearTimer(GeyserNearSpawnTimer);
	GetWorldTimerManager().SetTimer(GeyserNearSpawnTimer, this, &AGeyserController::SpawnGeyser, GeyserNearlyReadyTime, false);
	const int32 RandomIndex = FMath::RandRange(0, GeyserSpawnpoints.Num() - 1);
	
	//if(!GeyserSpawnpoints.IsValidIndex(RandomIndex)) return; //This is not needed because unless random shattered this should always work.
	
	
	SmokingGeiser = GeyserSpawnpoints[RandomIndex];
	SmokingGeiser->SetGeyserState(EGeyserStates::Smoking);
	GeyserNearlyReady(SmokingGeiser);
	
	
}

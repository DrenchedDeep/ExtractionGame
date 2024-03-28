


#include "GeyserController.h"

TWeakObjectPtr<AGeyserController> AGeyserController::SingletonInstance = nullptr;


void AGeyserController::GeyserNearlyReadyEveryone_Implementation()
{
	GeyserNearlyReady(SmokingGeiser);
}

void AGeyserController::BeginPlay()
{
	Super::BeginPlay();
	SingletonInstance = this;
	if(HasAuthority())
	{
		for(const auto Geyser : GeyserSpawnpoints)
		{
			Geyser->SetGeyserState(EGeyserStates::Deactive);
			Geyser->InitGeyser(GeyserSmokeDuration, GeyserActiveDuration);
		}
		GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::NearlySpawnGeyser, GeyserSpawnDelay - GeyserNearlyReadyTime, false);
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
	
	GetWorldTimerManager().ClearTimer(GeyserSpawnTimer);
	GetWorldTimerManager().SetTimer(GeyserSpawnTimer, this, &AGeyserController::NearlySpawnGeyser, GeyserSpawnDelay - GeyserNearlyReadyTime, false);
}

void AGeyserController::NearlySpawnGeyser()
{
	GetWorldTimerManager().ClearTimer(GeyserNearSpawnTimer);
	GetWorldTimerManager().SetTimer(GeyserNearSpawnTimer, this, &AGeyserController::SpawnGeyser, GeyserNearlyReadyTime, false);
	const int32 RandomIndex = FMath::RandRange(0, GeyserSpawnpoints.Num() - 1);
	
	//if(!GeyserSpawnpoints.IsValidIndex(RandomIndex)) return; //This is not needed because unless random shattered this should always work.
	
	
	SmokingGeiser = GeyserSpawnpoints[RandomIndex];
	SmokingGeiser->SetGeyserState(EGeyserStates::Smoking);
	GeyserNearlyReadyEveryone();
}

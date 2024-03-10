


#include "GeyserSpawnpoint.h"

#include "Net/UnrealNetwork.h"

void AGeyserSpawnpoint::ActivateGeyser()
{
	bGeyserActivated = true;
	OnRep_GeyserSpawned();
}

void AGeyserSpawnpoint::DeactivateGeyser()
{
	bGeyserActivated = false;
	OnRep_GeyserSpawned();
}

// Sets default values
AGeyserSpawnpoint::AGeyserSpawnpoint()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGeyserSpawnpoint::OnRep_GeyserSpawned()
{
	OnGeyserStatusUpdated();
}

void AGeyserSpawnpoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGeyserSpawnpoint, bGeyserActivated);
}


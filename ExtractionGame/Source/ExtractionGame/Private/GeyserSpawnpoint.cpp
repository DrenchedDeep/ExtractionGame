


#include "GeyserSpawnpoint.h"
#include "GeyserController.h"
#include "Net/UnrealNetwork.h"


void AGeyserSpawnpoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGeyserSpawnpoint, State);
}

void AGeyserSpawnpoint::SetGeyserState(EGeyserStates NewState)
{
	if(NewState != State)
	{
		State = NewState;
		OnGeyserStatusUpdated();
	}
	

	if(State == EGeyserStates::Smoking)
	{
		CurrentGeyserTime = 0;
	//	GetWorld()->GetTimerManager().SetTimer(	GeyserTimer, this, &AGeyserSpawnpoint::GeyserTick, 1.f, true);
	}
}

void AGeyserSpawnpoint::ResetGeyser()
{
	SetGeyserState(EGeyserStates::Deactive);
}

void AGeyserSpawnpoint::InitGeyser(float smokeTime, float activeTime)
{
	SmokeTime = smokeTime;
	ActiveTime = activeTime;
}

void AGeyserSpawnpoint::OnRep_GeyserState()
{
	OnGeyserStatusUpdated();
}

void AGeyserSpawnpoint::GeyserTick()
{
	CurrentGeyserTime++;

	if(State == EGeyserStates::Smoking && CurrentGeyserTime >= SmokeTime)
	{
		SetGeyserState(EGeyserStates::Active);
	}

	
	if(State == EGeyserStates::Active && CurrentGeyserTime >= SmokeTime + ActiveTime)
	{
		ResetGeyser();
		GetWorld()->GetTimerManager().ClearTimer(GeyserTimer);
	}
}

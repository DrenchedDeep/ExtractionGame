


#include "TDMPlayerState.h"

#include "Net/UnrealNetwork.h"

void ATDMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMPlayerState, Kills);
	DOREPLIFETIME(ATDMPlayerState, Deaths);
	DOREPLIFETIME(ATDMPlayerState, TeamID);
}

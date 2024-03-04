


#include "Core/Managers/PartyManager.h"

#include "Core/MainMenu/PlayerStand.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

APartyManager::APartyManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void APartyManager::OnRep_PartyPlayers()
{
	UpdatePlayers();
}

void APartyManager::AddPlayer(APlayerController* PlayerController)
{
	if(AlreadyHasPlayer(PlayerController->PlayerState) || !HasAuthority())
	{
		return; 
	}

	const FPartyPlayer Player(PlayerController->PlayerState, PlayerController->IsLocalController());
	PartyPlayers.Add(Player);
	OnRep_PartyPlayers();
}

void APartyManager::RemovePlayer(APlayerController* PlayerController)
{

}

void APartyManager::UpdatePlayerReadyStatus(APlayerState* PlayerState, bool bReady)
{
	for(int32 i = 0; i < PartyPlayers.Num(); i++)
	{
		if(PartyPlayers[i].PlayerState == PlayerState)
		{
			PartyPlayers[i].bIsReady = bReady;
			OnRep_PartyPlayers();
			break;
		}
	}
}

void APartyManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APartyManager, PartyPlayers);
}

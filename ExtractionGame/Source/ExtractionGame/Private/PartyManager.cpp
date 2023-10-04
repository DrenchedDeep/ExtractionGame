


#include "PartyManager.h"

#include "PlayerStand.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

APartyManager::APartyManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void APartyManager::OnRep_PartyPlayers()
{
	for(int32 i = 0; i < PartyPlayers.Num(); i++)
	{
		if(PartyPlayers[i].PlayerStand)
		{
			PartyPlayers[i].PlayerStand->Validate(PartyPlayers[i]);
		}
	}
}

void APartyManager::AddPlayer(APlayerController* PlayerController, APlayerStand* PlayerStand)
{
	if(AlreadyHasPlayer(PlayerController->PlayerState) || !HasAuthority())
	{
		return; 
	}

	const FPartyPlayer Player(PlayerController->PlayerState, PlayerStand, PlayerController->IsLocalController());
	PartyPlayers.Add(Player);
	OnRep_PartyPlayers();
}

void APartyManager::RemovePlayer(APlayerController* PlayerController)
{

}

void APartyManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APartyManager, PartyPlayers);
}

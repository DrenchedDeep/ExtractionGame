


#include "Core/Managers/PartyManager.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"
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

	if(UExtractionGameInstance* GameInstance = GetGameInstance<UExtractionGameInstance>())
	{
		TArray<FString> PlayerNames;

		for(auto Player : PartyPlayers)
		{
			if(Player.PlayerState)
			{
				PlayerNames.Add(Player.PlayerState->GetPlayerName());
			}
		}
		
		GameInstance->BuildPartySessionData(PlayerNames, PartyID);
		UE_LOG(LogTemp, Warning, TEXT("Party ID: %d"), PartyID);
	}
}

void APartyManager::AddPlayer(APlayerController* PlayerController)
{
	if(AlreadyHasPlayer(PlayerController->PlayerState) || !HasAuthority())
	{
		return; 
	}

	if(PartyID == 0)
	{
		PartyID = FMath::RandRange(0, 10000);
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

void APartyManager::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		PartyID = FMath::RandRange(0, 10000);
	}
}


void APartyManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APartyManager, PartyPlayers);
	DOREPLIFETIME(APartyManager, PartyID);
}


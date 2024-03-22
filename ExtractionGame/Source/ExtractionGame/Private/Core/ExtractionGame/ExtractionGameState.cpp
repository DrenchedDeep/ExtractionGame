


#include "Core/ExtractionGame/ExtractionGameState.h"

#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Net/UnrealNetwork.h"

AExtractionGameState::AExtractionGameState()
{
	
}

void AExtractionGameState::OnPlayerKilled(const FString& KillerName, const FString& VictimName,
	const FString& DeathCause)
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if(AExtractionGamePlayerController* PlayerController = Cast<AExtractionGamePlayerController>(Iterator->Get()))
		{
			PlayerController->Client_OnPlayerKilled(KillerName, VictimName, DeathCause);
		}
	}
}

FReplicatedPartyInfo AExtractionGameState::GetPartyByID(int32 PartyID)
{
	for(auto& Party : ReplicatedParties)
	{
		if(Party.PartyID == PartyID)
		{
			return Party;
		}
	}

	return FReplicatedPartyInfo();
}


void AExtractionGameState::UpdateParties(TArray<FInGameParty> Parties)
{
	TArray<FReplicatedPartyInfo> NewReplicatedParties;
	
	for(int32 i = 0; i < Parties.Num(); i++)
	{
		FReplicatedPartyInfo NewParty;

		for(const auto Player : Parties[i].Players)
		{
			NewParty.PartyMembers.Add(Player->PlayerState);
		}

		NewParty.PartyID = Parties[i].PartyID;
		NewReplicatedParties.Add(NewParty);
	}
	
	
	ReplicatedParties = NewReplicatedParties;
}

void AExtractionGameState::SetState(TEnumAsByte<EGameModeState> NewState)
{
	ExtractionGameState = NewState;
	OnExtractionGameStateUpdated();
}

void AExtractionGameState::SetTopThreePlayers(TArray<AExtractionGamePlayerState*> NewTopThreePlayers)
{
	TopThreePlayers = NewTopThreePlayers;
	OnTopThreePlayersUpdated();
}

void AExtractionGameState::SetBlockMovement(bool bInBlockMovement)
{
	bBlockMovement = bInBlockMovement;
	OnRep_BlockMovement();
}

void AExtractionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGameState, GooberSounds);
	DOREPLIFETIME(AExtractionGameState, ReplicatedParties);
	DOREPLIFETIME(AExtractionGameState, MatchTimer);
	DOREPLIFETIME(AExtractionGameState, ExtractionGameState);
	DOREPLIFETIME(AExtractionGameState, TopThreePlayers);
}

void AExtractionGameState::BeginPlay()
{
	Super::BeginPlay();

	GooberSounds = GetWorld()->SpawnActor<AGooberSoundsManager>(GooberSoundSubclass);
}

void AExtractionGameState::OnRep_ExtractionGameState()
{
	OnExtractionGameStateUpdated();
}

void AExtractionGameState::OnRep_TopThreePlayers()
{
	OnTopThreePlayersUpdated();
}

void AExtractionGameState::OnRep_BlockMovement()
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if(bBlockMovement && PC)
		{
			PC->SetInputMode(	FInputModeUIOnly());
			PC->StopMovement();
		}
	}
}



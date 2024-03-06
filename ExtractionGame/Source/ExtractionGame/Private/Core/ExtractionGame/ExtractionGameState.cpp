


#include "Core/ExtractionGame/ExtractionGameState.h"

#include "Net/UnrealNetwork.h"

AExtractionGameState::AExtractionGameState()
{
	
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

void AExtractionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGameState, GooberSounds);
	DOREPLIFETIME(AExtractionGameState, ReplicatedParties);
}

void AExtractionGameState::BeginPlay()
{
	Super::BeginPlay();

	GooberSounds = GetWorld()->SpawnActor<AGooberSoundsManager>(GooberSoundSubclass);
}

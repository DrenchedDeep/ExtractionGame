


#include "TDMGameState.h"

#include "ExtractionGame/ExtractionGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ATDMGameState::BeginPlay()
{
	Super::BeginPlay();

	RedTeam.TeamID = 1;
	BlueTeam.TeamID = 0;
}

void ATDMGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	TArray<AActor*> Spawnpoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnpoint::StaticClass(), Spawnpoints);

	for(int i = 0; i < Spawnpoints.Num(); i++)
	{
		if(ASpawnpoint* Spawnpoint = Cast<ASpawnpoint>(Spawnpoints[i]))
		{
			if(Spawnpoint->TeamID == 0)
			{
				BlueTeamDefaultsSpawnpoints.Add(Spawnpoint);
			}
			else if(Spawnpoint->TeamID == 1)
			{
				RedTeamDefaultsSpawnpoints.Add(Spawnpoint);
			}
		}
	}
}

int32 ATDMGameState::RegisterPlayerToTeam(APlayerController* PlayerController)
{
	const uint8 TeamToJoin = GetBestTeam();

	ATDMPlayerState* PlayerState = Cast<ATDMPlayerState>(PlayerController->PlayerState);
	
	if(TeamToJoin == 0)
	{
		BlueTeam.PlayerStates.Add(PlayerState);
	}
	else if(TeamToJoin == 1)
	{
		RedTeam.PlayerStates.Add(PlayerState);
	}

	PlayerState->TeamID = TeamToJoin;

	return TeamToJoin;
}


void ATDMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMGameState, RedTeam);
	DOREPLIFETIME(ATDMGameState, BlueTeam);
}

void ATDMGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();

	MatchStateChanged(GetMatchState());
}

void ATDMGameState::HandleMatchIsWaitingToStart()
{
}

void ATDMGameState::HandleMatchHasStarted()
{
}

void ATDMGameState::HandleMatchHasEnded()
{
}

void ATDMGameState::HandleLeavingMap()
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		
		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(PlayerController))
		{
			PC->Client_ReturnToLobby();
		}
	}
}


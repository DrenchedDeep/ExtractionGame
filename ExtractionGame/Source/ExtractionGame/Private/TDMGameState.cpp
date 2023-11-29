


#include "TDMGameState.h"

#include "ExtractionGameHUD.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "ExtractionGame/ExtractionGamePlayerController.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ATDMGameState::BeginPlay()
{
	Super::BeginPlay();

	RedTeam.TeamID = 1;
	BlueTeam.TeamID = 0;
	ElapsedTime = MatchLength;
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

void ATDMGameState::OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause)
{
	if(const ATDMPlayerState* KillerPlayerState = GetPlayerStateByName(KillerName))
	{
		if(KillerPlayerState->TeamID == 0)
		{
			BlueTeam.Score++;
		}
		else if(KillerPlayerState->TeamID == 1)
		{
			RedTeam.Score++;
		}
	}

	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if(AExtractionGamePlayerController* PlayerController = Cast<AExtractionGamePlayerController>(Iterator->Get()))
		{
			UE_LOG(LogTemp, Warning, TEXT("fgjf"));
			PlayerController->Client_OnPlayerKilled(KillerName, VictimName, DeathCause);
		}
	}
}


void ATDMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMGameState, RedTeam);
	DOREPLIFETIME(ATDMGameState, BlueTeam);
	DOREPLIFETIME(ATDMGameState, WinningTeamID);
}

void ATDMGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();

	MatchStateChanged(GetMatchState());

	if(GetMatchState() == MatchState::WaitingPostMatch)
	{
		
	}
}

void ATDMGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}

void ATDMGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ATDMGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void ATDMGameState::HandleLeavingMap()
{
	Super::HandleLeavingMap();
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		
		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(PlayerController))
		{
			PC->Client_ReturnToLobby();
		}
	}
}

void ATDMGameState::OnRep_ElapsedTime()
{
	Super::OnRep_ElapsedTime();

	if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(PC->GetHUD()))
		{
			HUD->UpdateMatchTimerText(ElapsedTime);
		}
	}
}

void ATDMGameState::DefaultTimer()
{
	if (IsMatchInProgress())
	{
		--ElapsedTime;
		if (GetNetMode() != NM_DedicatedServer)
		{
			OnRep_ElapsedTime();
		}
		
		if(ElapsedTime <= 0)
		{
			EndGame();
			GetWorldTimerManager().ClearTimer(TimerHandle_DefaultTimer);
			return;
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AGameState::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation() / GetWorldSettings()->DemoPlayTimeDilation, true);
}

void ATDMGameState::OnRep_WinningTeamID()
{
	if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(PC->GetHUD()))
		{
			ATDMPlayerState* PlayerState = Cast<ATDMPlayerState>(PC->PlayerState);
			FString GameOutcome = WinningTeamID == PlayerState->TeamID ? "YOU WON" : "YOU LOST";

			if(WinningTeamID == 3)
			{
				GameOutcome = "TIED";
			}
			HUD->EndGame(GameOutcome);
		}
	}
}

void ATDMGameState::EndGame()
{
	for(int i = 0; i < PlayerArray.Num(); i++)
	{
		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(PlayerArray[i]->GetPlayerController()))
		{
			PC->Client_ReturnToLobby();
		}
	}

	SetMatchState(MatchState::WaitingPostMatch);
}



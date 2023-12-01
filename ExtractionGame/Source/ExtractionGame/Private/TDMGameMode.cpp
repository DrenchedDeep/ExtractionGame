#include "TDMGameMode.h"
#include "TDMGameState.h"
#include "ExtractionGame/ExtractionGameInstance.h"
#include "ExtractionGame/ExtractionGamePlayerController.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

void ATDMGameMode::RespawnAllPlayerPawns()
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if(PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			
			if(PlayerPawn)
			{
				PlayerController->UnPossess();
				PlayerPawn->Destroy();
			}

			const ATDMPlayerState* PlayerState = Cast<ATDMPlayerState>(PlayerController->PlayerState);
			SpawnPlayer(PlayerController, PlayerState->TeamID);
		}
	}
}


void ATDMGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ATDMGameState* TDMGameState = Cast<ATDMGameState>(GetWorld()->GetGameState());
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	if(TDMGameState->GetMatchState() == MatchState::InProgress)
	{
		uint8 TeamID = TDMGameState->RegisterPlayerToTeam(NewPlayer);
		SpawnPlayer(NewPlayer, TeamID);

		//match is in progress, kick user
		//GameSession->KickPlayer(NewPlayer, FText::FromString("Match is in progress"));
	}
	else if(TDMGameState->GetMatchState() == MatchState::WaitingToStart || TDMGameState->GetMatchState() == MatchState::EnteringMap)
	{
		uint8 TeamID = TDMGameState->RegisterPlayerToTeam(NewPlayer);

		if(TDMGameState->PlayerArray.Num() >= 2) //GameInstance->SESSION_PLAYERCOUNT)
		{
			//match is waiting to start, but there are already 2 players, start match
			TDMGameState->SetMatchState(MatchState::InProgress);

		//	GameInstance->StartSession();

			RespawnAllPlayerPawns();
		}
		else
		{
			//match is waiting to start, spawn player
			SpawnPlayer(NewPlayer, TeamID);
		}
	}
}

void ATDMGameMode::SpawnPlayer(APlayerController* NewPlayer, int32 TeamID)
{
	ATDMGameState* TDMGameState = Cast<ATDMGameState>(GetWorld()->GetGameState());
	APlayerStart* SpawnPoint = TDMGameState->GetTeamSpawnPoint(TeamID);

	if(!SpawnPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn point found for team %d"), TeamID);
		SpawnPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawning player at %s"), *SpawnPoint->GetActorLocation().ToString());

	APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor(Player, &SpawnPoint->GetActorTransform()));

	if(PlayerPawn)
	{
		NewPlayer->Possess(PlayerPawn);
	//	PlayerPawn->SetOwner(NewPlayer);
	}
}

void ATDMGameMode::KickAllPlayers()
{
	FTimerDelegate KickDelegate = FTimerDelegate::CreateUObject(this, &ATDMGameMode::KickAllPlayersTimer);
	GetWorld()->GetTimerManager().SetTimer(KickEveryoneTimerHandle, KickDelegate, 15.0f, false);
}

void ATDMGameMode::KickAllPlayersTimer()
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if(PlayerController)
		{
			if(AExtractionGamePlayerController* ExtractionGamePlayerController = Cast<AExtractionGamePlayerController>(PlayerController))
			{
				ExtractionGamePlayerController->Client_ReturnToLobby();
			}
		}
	}


	//destroy session
}

#include "TDMGameMode.h"
#include "TDMGameState.h"
#include "ExtractionGame/ExtractionGameInstance.h"
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

	UE_LOG(LogTemp, Warning, TEXT("%s"), *TDMGameState->GetMatchState().ToString());
	if(TDMGameState->GetMatchState() == MatchState::InProgress)
	{
		//match is in progress, kick user
		GameSession->KickPlayer(NewPlayer, FText::FromString("Match is in progress"));
	}
	else if(TDMGameState->GetMatchState() == MatchState::WaitingToStart || TDMGameState->GetMatchState() == MatchState::EnteringMap)
	{
		uint8 TeamID = TDMGameState->RegisterPlayerToTeam(NewPlayer);

		if(TDMGameState->PlayerArray.Num() >= 2)
		{
			//match is waiting to start, but there are already 2 players, start match
			TDMGameState->SetMatchState(MatchState::InProgress);

			UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
			GameInstance->StartSession();

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
	APlayerStart* SpawnPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	if(!SpawnPoint)
	{
//		UE_LOG(LogTemp, Warning, TEXT("No spawn point found for team %d"), TeamID);
	//	SpawnPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawning player at %s"), *SpawnPoint->GetActorLocation().ToString());

	APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor(Player, &SpawnPoint->GetActorTransform()));

	if(PlayerPawn)
	{
		NewPlayer->Possess(PlayerPawn);
	//	PlayerPawn->SetOwner(NewPlayer);
	}
}

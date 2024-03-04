#include "Core/MainMenu/MainMenuGameModeBase.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Core/ExtractionGame/ExtractionGameState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuGameModeBase::SetupMemberModel(APlayerState* PlayerState, const FString& Username)
{
	PlayerState->SetPlayerName(Username);
}

void AMainMenuGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(!PartyManager)
	{
		PartyManager = GetWorld()->SpawnActor<APartyManager>(PartyManagerClass);

		if(AMainMenuGameState* MainMenuGameState = Cast<AMainMenuGameState>(GameState))
		{
			MainMenuGameState->PartyManager = PartyManager;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	if(SpaceShips.Num() <= 0)
	{
		TArray<AActor*> Stands;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainMenuSpaceShip::StaticClass(),Stands);

		for(auto Stand : Stands)
		{
			AMainMenuSpaceShip* PlayerStand = Cast<AMainMenuSpaceShip>(Stand);
			SpaceShips.Add(PlayerStand);
			UE_LOG(	LogTemp, Warning, TEXT("Added Stand"));
		}
	}

	for(int32 i = 0; i < SpaceShips.Num(); i++)
	{
		if(SpaceShips[i]->bHasPlayer)
		{
			continue;
		}

		UE_LOG(	LogTemp, Warning, TEXT("Added 2"));

		SpaceShips[i]->bHasPlayer = true;
		SpaceShips[i]->SetOwner(NewPlayer->PlayerState);
		break;
	}

	PartyManager->AddPlayer(NewPlayer);
}

void AMainMenuGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AMainMenuGameState* MainMenuGameState = Cast<AMainMenuGameState>(GameState);
	if(MainMenuGameState && MainMenuGameState->bIsConnectingToGameSession && MainMenuGameState->ConnectionInfo.bIsValid)
	{
		if(MainMenuGameState->PlayerArray.Num() <= 1)
		{
			if(UExtractionGameInstance* ExtractionGameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
			{
				ExtractionGameInstance->JoinSession(false);
				UE_LOG(LogTemp, Warning, TEXT("host is tryna join"));
			}
		}
	}
}

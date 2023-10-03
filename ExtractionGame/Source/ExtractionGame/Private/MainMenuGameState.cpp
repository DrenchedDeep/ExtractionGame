#include "MainMenuGameState.h"
#include "MainMenuHUD.h"
#include "MainMenuPlayerState.h"
#include "ExtractionGame/ExtractionGameInstance.h"
#include "ExtractionGame/MainMenuGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMainMenuGameState::AMainMenuGameState(const FObjectInitializer&  ObjectInitializer)
	: Super (ObjectInitializer)
{
	
}

void AMainMenuGameState::FindSession()
{
	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		GameInstance->JoinSession(ConnectionInfo);
	}
}

void AMainMenuGameState::Multicast_JoinSession_Implementation(FClientConnectionInfo ConnectInfo)
{
	if(!ConnectInfo.bIsValid)
	{
		return;
	}
	
	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		GameInstance->JoinSession(ConnectInfo);
	}
}

void AMainMenuGameState::Multicast_SendConnectionInfo_Implementation(FClientConnectionInfo ConnectInfo)
{
	if(!ConnectInfo.bIsValid)
	{
		return;
	}

	ConnectionInfo = ConnectInfo;
	GetWorld()->GetTimerManager().SetTimer(FindSessionTimer, this, &AMainMenuGameState::FindSession, 5.f, false);
}

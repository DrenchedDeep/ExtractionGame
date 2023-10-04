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

void AMainMenuGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMenuGameState, PartyManager);
}

void AMainMenuGameState::Multicast_JoinSession_Implementation(FClientConnectionInfo ConnectInfo)
{
	if(!ConnectInfo.bIsValid)
	{
		return;
	}

	if(HasAuthority())
	{
		return;
	}
	
	ConnectionInfo = ConnectInfo;
	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		GameInstance->JoinSession();
	}
}

void AMainMenuGameState::Multicast_SendConnectionInfo_Implementation(FClientConnectionInfo ConnectInfo)
{
	if(!ConnectInfo.bIsValid)
	{
		return;
	}

	ConnectionInfo = ConnectInfo;
}

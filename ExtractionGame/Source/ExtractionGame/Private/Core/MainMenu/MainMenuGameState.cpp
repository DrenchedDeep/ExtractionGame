#include "Core/MainMenu/MainMenuGameState.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMainMenuGameState::AMainMenuGameState(const FObjectInitializer&  ObjectInitializer)
	: Super (ObjectInitializer)
{
	
}

void AMainMenuGameState::BeginPlay()
{
	Super::BeginPlay();
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
	
	ConnectionInfo = ConnectInfo;


	if(HasAuthority())
	{
		
		return;
	}
	
	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		GameInstance->JoinSession(false);
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

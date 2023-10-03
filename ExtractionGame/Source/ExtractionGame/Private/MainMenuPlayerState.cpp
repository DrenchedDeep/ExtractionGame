


#include "MainMenuPlayerState.h"

#include "ExtractionGame/ExtractionGameInstance.h"
#include "ExtractionGame/MainMenuGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void AMainMenuPlayerState::Server_SetUsername_Implementation(const FString& Username)
{
	SetPlayerName(Username);

	if(AMainMenuGameModeBase* Gamemode = Cast<AMainMenuGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		Gamemode->SetupMemberModel(this, Username);
	}
}

void AMainMenuPlayerState::Server_ToggleReadyStatus_Implementation(bool bReady)
{
	bPlayerReadyForMatch = bReady;
}

void AMainMenuPlayerState::OnRep_PlayerReady()
{
	//
}

void AMainMenuPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMenuPlayerState, bPlayerReadyForMatch);
}

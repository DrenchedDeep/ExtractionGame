


#include "Core/MainMenu/MainMenuPlayerState.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Core/MainMenu/MainMenuGameModeBase.h"
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

	UE_LOG(LogTemp, Warning, TEXT("hello"));
	if(AMainMenuGameState* GameState = Cast<AMainMenuGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameState->PartyManager->UpdatePlayerReadyStatus(this, bReady);
	}
}

void AMainMenuPlayerState::OnRep_PlayerReady()
{

}

void AMainMenuPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMenuPlayerState, bPlayerReadyForMatch);
}

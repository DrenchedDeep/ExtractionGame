#include "MainMenuPlayerController.h"

#include "MainMenuPlayerState.h"
#include "ExtractionGame/ExtractionGameInstance.h"


void AMainMenuPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		const FString PlayerUsername = GameInstance->GetPlayerUsername();

		if(AMainMenuPlayerState* MainMenuPlayerState = Cast<AMainMenuPlayerState>(PlayerState))
		{
			MainMenuPlayerState->Server_SetUsername(PlayerUsername);
		}
		else
		{
			GLog->Log("opiop");
		}
	}
	else
	{
		GLog->Log("jkjl");

	}
}

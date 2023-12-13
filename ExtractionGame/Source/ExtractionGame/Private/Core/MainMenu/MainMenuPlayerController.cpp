#include "Core/MainMenu/MainMenuPlayerController.h"

#include "Core/MainMenu/MainMenuPlayerState.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"


void AMainMenuPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	if(GameInstance)
	{
		const FString PlayerUsername = GameInstance->GetPlayerUsername();

		if(AMainMenuPlayerState* MainMenuPlayerState = Cast<AMainMenuPlayerState>(PlayerState))
		{
			MainMenuPlayerState->Server_SetUsername(PlayerUsername);
		}
	}
	
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(IsLocalController())
	{
		PlayerStashManager = Cast<APlayerStashManager>(GetWorld()->SpawnActor(PlayerStashManagerClass));
	}
}

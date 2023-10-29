#include "MainMenuPlayerController.h"

#include "MainMenuPlayerState.h"
#include "ExtractionGame/ExtractionGameInstance.h"


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
		UE_LOG(LogTemp, Warning, TEXT("Player Controller Begin Play"));
		PlayerStashManager = Cast<APlayerStashManager>(GetWorld()->SpawnActor(PlayerStashManagerClass));
	}
}

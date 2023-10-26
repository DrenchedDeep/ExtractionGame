// Copyright Epic Games, Inc. All Rights Reserved.


#include "ExtractionGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "ExtractionGameInstance.h"

void AExtractionGamePlayerController::ReturnToLobby()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());

	if(GameInstance->CurrentSession)
	{
		GameInstance->DestroySession();
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), "LVL_MainMenu?listen");
}

void AExtractionGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AExtractionGamePlayerController::ClientWasKicked_Implementation(const FText& KickReason)
{
	UGameplayStatics::OpenLevel(GetWorld(), "LVL_Startup");
}

// Copyright Epic Games, Inc. All Rights Reserved.


#include "ExtractionGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "ExtractionGameCharacter.h"
#include "ExtractionGameInstance.h"

void AExtractionGamePlayerController::ReturnToLobby()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());

	GameInstance->OnRaidOver(true, 25.f);
	GameInstance->ShowLoadingScreen();
	
	if(GameInstance->CurrentSession)
	{
		GameInstance->DestroySession();
	}

	//for now, we'll save the inventory for testing but we need to delete the saved inventory if we leave the match by choice
	if(AExtractionGameCharacter* PlayerCharacter =  Cast<AExtractionGameCharacter>(GetPawn()))
	{
		GameInstance->BuildPlayerSessionData(PlayerCharacter->InventoryComponent->InventoryItems, TArray<FName>());
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

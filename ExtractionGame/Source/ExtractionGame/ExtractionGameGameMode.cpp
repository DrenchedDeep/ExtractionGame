// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtractionGameGameMode.h"
#include "ExtractionGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AExtractionGameGameMode::AExtractionGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AExtractionGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	
}

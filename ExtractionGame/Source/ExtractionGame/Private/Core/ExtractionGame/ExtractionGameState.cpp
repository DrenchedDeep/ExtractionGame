


#include "Core/ExtractionGame/ExtractionGameState.h"

#include "Net/UnrealNetwork.h"

AExtractionGameState::AExtractionGameState()
{
	
}

void AExtractionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGameState, GooberSounds);
}

void AExtractionGameState::BeginPlay()
{
	Super::BeginPlay();

	GooberSounds = GetWorld()->SpawnActor<AGooberSoundsManager>(GooberSoundSubclass);
}

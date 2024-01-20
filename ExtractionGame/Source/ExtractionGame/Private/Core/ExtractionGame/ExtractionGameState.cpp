


#include "Core/ExtractionGame/ExtractionGameState.h"

AExtractionGameState::AExtractionGameState()
{
	ItemReplicationManager = CreateDefaultSubobject<UItemReplicationManager>(TEXT("ItemReplicationManager"));
}

void AExtractionGameState::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
//		ItemReplicationManager->Init();
	}
}

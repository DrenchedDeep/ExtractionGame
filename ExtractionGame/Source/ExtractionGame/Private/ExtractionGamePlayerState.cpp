


#include "ExtractionGamePlayerState.h"

#include "Net/UnrealNetwork.h"

void AExtractionGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGamePlayerState, ExtractedEssence);
	DOREPLIFETIME(AExtractionGamePlayerState, Kills);
	DOREPLIFETIME(AExtractionGamePlayerState, Deaths);
}

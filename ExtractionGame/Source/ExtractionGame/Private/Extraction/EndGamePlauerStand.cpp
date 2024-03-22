


#include "Extraction/EndGamePlauerStand.h"

#include "Net/UnrealNetwork.h"

void AEndGamePlauerStand::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEndGamePlauerStand, StandOwner);
}

AEndGamePlauerStand::AEndGamePlauerStand()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AEndGamePlauerStand::SetStandOwner(AExtractionGamePlayerState* NewOwner)
{
	StandOwner = NewOwner;
	OnStandOwnerChanged();
}

void AEndGamePlauerStand::OnRep_StandOwner()
{
	OnStandOwnerChanged();
}

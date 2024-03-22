


#include "Core/ExtractionGame/EndMatchController.h"

#include "Net/UnrealNetwork.h"


void AEndMatchController::EndMatch(TArray<AExtractionGamePlayerState*> TopThreePlayers)
{
	for(int32 i = 0; i < Stands.Num(); i++)
	{
		if (i < TopThreePlayers.Num())
		{
			Stands[i]->SetStandOwner(TopThreePlayers[i]);
		}
		else
		{
			Stands[i]->SetStandOwner(nullptr);
		}
	}
	MatchEnded++;
	OnMatchEnded();
}

void AEndMatchController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEndMatchController, MatchEnded);
}

AEndMatchController::AEndMatchController()
{
}

void AEndMatchController::OnRep_MatchEnded()
{
	OnMatchEnded();
}

void AEndMatchController::ContinueToNextStage()
{
	if (LocalPlayerStage == ELocalPlayerStage::WaitingToStart)
	{
		LocalPlayerStage = ELocalPlayerStage::LookingAtTopPlayers;
	}
	else if (LocalPlayerStage == ELocalPlayerStage::LookingAtTopPlayers)
	{
		LocalPlayerStage = ELocalPlayerStage::LookingAtStats;
	}
	else if (LocalPlayerStage == ELocalPlayerStage::LookingAtStats)
	{
		LocalPlayerStage = ELocalPlayerStage::LoadingMainMenu;
	}
}

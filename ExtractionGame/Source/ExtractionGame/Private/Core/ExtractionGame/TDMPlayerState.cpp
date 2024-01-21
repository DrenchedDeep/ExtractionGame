#include "Core/ExtractionGame/TDMPlayerState.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/ExtractionGameHUD.h"

void ATDMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMPlayerState, Kills);
	DOREPLIFETIME(ATDMPlayerState, Deaths);
	DOREPLIFETIME(ATDMPlayerState, TeamID);
}

void ATDMPlayerState::OnRep_Kills()
{
	AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(	GetPlayerController());

	if(PC)
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(PC->GetHUD()))
		{
			HUD->UpdateScore(Kills, Deaths);
		}
	}
}


void ATDMPlayerState::OnRep_Deaths()
{
	AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(	GetPlayerController());

	if(PC)
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(PC->GetHUD()))
		{
			HUD->UpdateScore(Kills, Deaths);
		}
	}
}

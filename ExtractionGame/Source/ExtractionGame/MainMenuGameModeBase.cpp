#include "MainMenuGameModeBase.h"

#include "ExtractionGameInstance.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
}

void AMainMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AMainMenuGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(PlayerStands.Num() <= 0)
	{
		TArray<AActor*> Stands;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStand::StaticClass(),Stands);

		for(auto Stand : Stands)
		{
			APlayerStand* PlayerStand = Cast<APlayerStand>(Stand);
			PlayerStands.Add(PlayerStand);
		}
	}

	for(auto PlayerStand : PlayerStands)
	{
		if(PlayerStand->bIsOccupied)
		{
			return;
		}
		PlayerStand->bIsOccupied = true;
		PlayerStand->OnRep_IsOccupied();
		break;
	}
}

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

	for(int32 i = 0; i < PlayerStands.Num(); i++)
	{
		if(PlayerStands[i]->bIsOccupied)
		{
			continue;
		}
		PlayerStands[i]->bIsOccupied = true;
		PlayerStands[i]->OwningClient = NewPlayer;
		PlayerStands[i]->OnRep_IsOccupied();
		break;
	}
}

void AMainMenuGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Warning, TEXT("log out"));
	for(int32 i = 0; i < PlayerStands.Num(); i++)
	{
		if(!PlayerStands[i]->OwningClient)
		{
			continue;
		}
		
		if(Exiting == PlayerStands[i]->OwningClient)
		{
			PlayerStands[i]->bIsOccupied = false;
			PlayerStands[i]->OwningClient = nullptr;
			PlayerStands[i]->OnRep_IsOccupied();
			break;
		}
	}
}

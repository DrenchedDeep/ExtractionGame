#include "MainMenuGameModeBase.h"
#include "ExtractionGameInstance.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuGameModeBase::SetupMemberModel(APlayerState* PlayerState, const FString& Username)
{	
	for(int32 i = 0; i < PlayerStands.Num(); i++)
	{
		if(PlayerState == PlayerStands[i]->OwningClient)
		{
			PlayerStands[i]->Username = Username;
			PlayerStands[i]->OnRep_Username();
		}
	}
}

void AMainMenuGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(!PartyManager)
	{
		PartyManager = GetWorld()->SpawnActor<APartyManager>();

		if(AMainMenuGameState* MainMenuGameState = Cast<AMainMenuGameState>(GameState))
		{
			MainMenuGameState->PartyManager = PartyManager;
		}
	}
	
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
		PlayerStands[i]->OwningClient = NewPlayer->PlayerState;
		PlayerStands[i]->SetOwner(NewPlayer);

		//we call on rep here because its not automatically called on server, and since parties are player hosted the server needs to see when a client joins
		PlayerStands[i]->OnRep_IsOccupied();
		PlayerStands[i]->OnRep_Username();
		PartyManager->AddPlayer(NewPlayer, PlayerStands[i]);
		break;
	}
}

void AMainMenuGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	for(int32 i = 0; i < PlayerStands.Num(); i++)
	{
		if(!PlayerStands[i]->OwningClient)
		{
			continue;
		}
		
		if(Exiting == PlayerStands[i]->OwningClient->GetOwningController())
		{
			PlayerStands[i]->bIsOccupied = false;
			PlayerStands[i]->OwningClient = nullptr;
			PlayerStands[i]->Username = "";
			
			PlayerStands[i]->OnRep_IsOccupied();
			PlayerStands[i]->OnRep_Username();
			break;
		}
	}
}

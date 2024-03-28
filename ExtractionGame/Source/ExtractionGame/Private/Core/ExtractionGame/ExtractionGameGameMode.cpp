// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ExtractionGame/ExtractionGameGameMode.h"

#include "ExtractionGamePlayerState.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Core/ExtractionGame/ExtractionGameState.h"
#include "Core/ExtractionGame/SpaceShip.h"
#include "Core/ExtractionGame/Spawnpoint.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerStart.h"

AExtractionGameGameMode::AExtractionGameGameMode()
	: Super()
{
	//bUseSeamlessTravel = true;
}

void AExtractionGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	RegisterPlayerEOS(NewPlayer);
}

void AExtractionGameGameMode::OnPartyInfoRecieved(APlayerController* Sender, FPartyInfo PartyInfo)
{
	int32 Index;
	if(HasParty(PartyInfo.PartyID, Index) && AllParties.IsValidIndex(Index))
	{
		AllParties[Index].Players.Add(Sender);

		if(AllParties[Index].Players.Num() >= AllParties[Index].ExpectedPlayerCount)
		{
			FVector Location;
			FRotator Rotation;
			GetPartySpawnLocation(Location, Rotation);
			int32 I = 0;
			for(auto Player : AllParties[Index].Players)
			{
				SpawnShip(Player, Location, Rotation, I);
				I++;
			}
		}
	}
	else
	{
		FInGameParty NewParty({Sender}, PartyInfo.PartyID, PartyInfo.PlayerNames.Num(), PartyInfo.PlayerNames.Num() == 1);
		AllParties.Add(NewParty);

		if(NewParty.bAllPlayersConnected)
		{
			FVector Location;
			FRotator Rotation;
			GetPartySpawnLocation(Location, Rotation);
			int32 I = 0;
			for(auto Player : NewParty.Players)
			{
				SpawnShip(Player, Location, Rotation, I);
				I++;
			}
		}
	}

	AExtractionGameState* GS = GetGameState<AExtractionGameState>();
	if(GS)
	{
		GS->UpdateParties(AllParties);
	}
}

void AExtractionGameGameMode::SetGameModeState(EGameModeState NewState)
{
	GameModeState = NewState;

	if(AExtractionGameState* GS = GetGameState<AExtractionGameState>())
	{
		GS->SetState(GameModeState);
	}

	if(GameModeState == EGameModeState::WaitingForPlayers)
	{
		GetWorld()->GetTimerManager().SetTimer(CheckToStartMatchTimerHandle,
			this, &AExtractionGameGameMode::CheckToStartMatch, 1.f, true);
	}
	else if(GameModeState == EGameModeState::Playing)
	{
		GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle,
			this, &AExtractionGameGameMode::TickMatch, 1.f, true);

		GetWorld()->GetTimerManager().ClearTimer(CheckToStartMatchTimerHandle);
	}
	else if(GameModeState == EGameModeState::EndingGame)
	{
		EndGame();
	}
	
}

void AExtractionGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetGameModeState(WaitingForPlayers);
	EndMatchController = GetWorld()->SpawnActor<AEndMatchController>(EndMatchControllerClass);
}


void AExtractionGameGameMode::RespawnShip(APlayerController* NewPlayer, int32 TeamID, FVector SpawnLocation,
	FRotator SpawnRotation) const
{
	FActorSpawnParameters Parm;
	Parm.bNoFail = true;

	const FVector PawnSpawnLocation = SpawnLocation == FVector::Zero() ? FVector(0,0, 10000) : SpawnLocation;
	if(APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor<ASpaceShip>(ShipClass, PawnSpawnLocation, SpawnRotation, Parm)))
	{
		NewPlayer->Possess(PlayerPawn);

		if(SpawnLocation == FVector::Zero())
		{
			if(ASpaceShip* Ship = Cast<ASpaceShip>(PlayerPawn))
			{
				Ship->bMoveToWorldSpawn = true;
			}
		}
	}
}

void AExtractionGameGameMode::SpawnShip(APlayerController* NewPlayer, const FVector StartLocation,
                                        const FRotator Rotator, int32 I)
{
	FVector SpawnLocation = StartLocation + FVector(1,0, 0) * ((I + 1) * 10000);
	
	FActorSpawnParameters Parm;
	Parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Parm.bNoFail = true;
	
	if(ASpaceShip* PlayerPawn = Cast<ASpaceShip>(GetWorld()->SpawnActor<ASpaceShip>(ShipClass, SpawnLocation, Rotator, Parm)))
	{
		NewPlayer->Possess(PlayerPawn);
		PlayerPawn->bBlockMovement = bBlockMovementTillMatchReady;
		SpawnedSpaceships++;
	}
}

void AExtractionGameGameMode::GetPartySpawnLocation(FVector& OutLocation, FRotator& OutRotation) const
{
	if(!HasAuthority())
	{
		OutLocation = FVector::ZeroVector;
		OutRotation = FRotator::ZeroRotator;
	}
	
	const UMapInfo* MapInfo = Cast<UExtractionGameInstance>(GetGameInstance())->GetMapInfo();
	
	const float distance = FMath::RandRange(MapInfo->InnerRing(), MapInfo->OuterRing());
	const float height = FMath::RandRange(MapInfo->MinHeight(), MapInfo->MaxHeight()) + MapInfo->AdditionalUpOffset();
	const float angle = FMath::RandRange(0.f, 6.28f);

	const FVector SpawnPoint(FMath::Cos(angle) * distance,FMath::Sin(angle) * distance,height);
	const FRotator Rotation(0,FMath::RadiansToDegrees(angle),0);

	OutLocation = SpawnPoint;
	OutRotation = Rotation;
}


void AExtractionGameGameMode::RegisterPlayerEOS(APlayerController* NewPlayer)
{
	if(NewPlayer != nullptr)
	{
		FUniqueNetIdRepl NetID;

		if(NewPlayer->IsLocalController())
		{
			ULocalPlayer *LocalPlayer = NewPlayer->GetLocalPlayer();

			if(LocalPlayer != nullptr)
			{
				NetID = LocalPlayer->GetPreferredUniqueNetId();
			}
			else
			{
				UNetConnection *RemoteNetConnection = Cast<UNetConnection>(NewPlayer->Player);

				if(IsValid(RemoteNetConnection))
				{
					NetID = RemoteNetConnection->PlayerId;
				}
			}
		}
		else
		{
			UNetConnection *RemoteNetConnection = Cast<UNetConnection>(NewPlayer->Player);

			if(IsValid(RemoteNetConnection))
			{
				NetID = RemoteNetConnection->PlayerId;
			}
		}

		const TSharedPtr<const FUniqueNetId> UniqueNetId = NetID.GetUniqueNetId();
		if(UniqueNetId != nullptr)
		{
			const IOnlineSubsystem *OnlineSubSystem = Online::GetSubsystem(NewPlayer->GetWorld());
			const IOnlineSessionPtr Session = OnlineSubSystem->GetSessionInterface();
			
			UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());
			
			if(!Session->RegisterPlayer(GameInstance->CurrentSession->SessionName, *UniqueNetId, false))
			{
				GameSession->KickPlayer(NewPlayer, FText::FromString("Failed to register player"));
			}

		}
	}
}

bool AExtractionGameGameMode::AllPlayersReady()
{
	const bool bPlayersReady = SpawnedSpaceships >= MinPlayersBeforeStarting;

	return bPlayersReady;
}

void AExtractionGameGameMode::EndGame()
{
	//get top three players
	TopThreePlayers.Reset();
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		const AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Iterator->Get());
		if(AExtractionGamePlayerState* PS = Cast<AExtractionGamePlayerState>(PC->PlayerState))
		{
			float Essence = PS->GetEssence();
			if(TopThreePlayers.Num() == 0)
			{
				TopThreePlayers.Add(PS);
			}
			else if(TopThreePlayers.Num() == 1)
			{
				if(Essence > TopThreePlayers[0]->GetEssence())
				{
					TopThreePlayers.Insert(PS, 0);
				}
				else
				{
					TopThreePlayers.Add(PS);
				}
			}
			else if(TopThreePlayers.Num() == 2)
			{
				if(Essence > TopThreePlayers[0]->GetEssence())
				{
					TopThreePlayers.Insert(PS, 0);
				}
				else if(Essence > TopThreePlayers[1]->GetEssence())
				{
					TopThreePlayers.Insert(PS, 1);
				}
				else
				{
					TopThreePlayers.Add(PS);
				}
			}
			else
			{
				if(Essence > TopThreePlayers[0]->GetEssence())
				{
					TopThreePlayers.Insert(PS, 0);
					TopThreePlayers.Pop();
				}
				else if(Essence > TopThreePlayers[1]->GetEssence())
				{
					TopThreePlayers.Insert(PS, 1);
					TopThreePlayers.Pop();
				}
				else if(Essence > TopThreePlayers[2]->GetEssence())
				{
					TopThreePlayers.Insert(PS, 2);
					TopThreePlayers.Pop();
				}
			}
		}
	}

	if(AExtractionGameState* GS = GetGameState<AExtractionGameState>())
	{
		GS->SetTopThreePlayers(TopThreePlayers);
		GS->SetBlockMovement(true);
	}

	
	GetWorld()->GetTimerManager().SetTimer(EndGameTimerHandle,
		this, &AExtractionGameGameMode::EndGameTimer, TimeBeforeEndGame, false);
}

void AExtractionGameGameMode::CheckToStartMatch()
{
	if(GetGameModeState() != EGameModeState::WaitingForPlayers)
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckToStartMatchTimerHandle);
		return;
	}

	if(AllPlayersReady())
	{
		SetGameModeState(EGameModeState::Playing);
	}
}

void AExtractionGameGameMode::TickMatch()
{
	if(GetGameModeState() != EGameModeState::Playing)
	{
		GetWorld()->GetTimerManager().ClearTimer(MatchTimerHandle);
		return;
	}

	MatchLength--;

	if(AExtractionGameState* GS = GetGameState<AExtractionGameState>())
	{
		GS->SetMatchTimer(MatchLength);
	}
	

	if(MatchLength == MatchShortTimeIndicator)
		LowMatchTime();
	

	if(MatchLength <= 0)
	{
		SetGameModeState(EGameModeState::EndingGame);
	}
}

void AExtractionGameGameMode::EndGameTimer()
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if(AExtractionGamePlayerController* PlayerController = Cast<AExtractionGamePlayerController>(Iterator->Get()))
		{
			if(PlayerController->GetPawn())
			{
				PlayerController->GetPawn()->Destroy();
			}
		}
	}

	SetGameModeState(PostEndingGame);
}

bool AExtractionGameGameMode::HasParty(int32 InID, int32& OutIndex)
{
	for(auto i = 0; i < AllParties.Num(); i++)
	{
		if(AllParties[i].PartyID == InID)
		{
			OutIndex = i;
			return true;
		}
	}
	
	OutIndex = 0;
	return false;
}


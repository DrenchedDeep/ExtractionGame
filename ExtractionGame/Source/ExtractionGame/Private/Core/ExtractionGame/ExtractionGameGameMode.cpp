// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ExtractionGame/ExtractionGameGameMode.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
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
	RegisterPlayerEOS(NewPlayer);

	Super::PostLogin(NewPlayer);
}

void AExtractionGameGameMode::OnPartyInfoRecieved(APlayerController* Sender, FPartyInfo PartyInfo)
{
	int32 Index;
	if(HasParty(PartyInfo.PartyID, Index) && AllParties.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("party with id already found"));
		AllParties[Index].Players.Add(Sender);

		if(AllParties[Index].Players.Num() >= AllParties[Index].ExpectedPlayerCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("party is good to spawn"));
			FVector Location;
			FRotator Rotation;
			GetPartySpawnLocation(Location, Rotation);
			int32 I = 0;
			for(auto Player : AllParties[Index].Players)
			{
				UE_LOG(LogTemp, Warning, TEXT("spawnin for every player"));
				SpawnShip(Player, Location, Rotation, I);
				I++;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("making party"));
		FInGameParty NewParty({Sender}, PartyInfo.PartyID, PartyInfo.PlayerNames.Num(), PartyInfo.PlayerNames.Num() == 1);
		AllParties.Add(NewParty);

		if(NewParty.bAllPlayersConnected)
		{
			UE_LOG(LogTemp, Warning, TEXT("all players connected"));
			FVector Location;
			FRotator Rotation;
			GetPartySpawnLocation(Location, Rotation);
			int32 I = 0;
			for(auto Player : AllParties[Index].Players)
			{
				UE_LOG(LogTemp, Warning, TEXT("spawnin for every player 2"));
				SpawnShip(Player, Location, Rotation, I);
				I++;
			}
		}
	}
}

void AExtractionGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AExtractionGameGameMode::RespawnShip(APlayerController* NewPlayer, int32 TeamID) const
{
	FActorSpawnParameters Parm;
	Parm.bNoFail = true;
	
	if(APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor<ASpaceShip>(DefaultPawnClass, FVector(0,0,100000), FRotator(0), Parm)))
	{
		NewPlayer->Possess(PlayerPawn);
	}
}

void AExtractionGameGameMode::SpawnShip(APlayerController* NewPlayer, const FVector StartLocation,
	const FRotator Rotator, int32 I)
{
	UE_LOG(LogTemp, Warning, TEXT("I is %d"), I);
	FVector SpawnLocation = StartLocation + FVector(1,0, 0) * ((I + 1) * 10000);
	
	FActorSpawnParameters Parm;
	Parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Parm.bNoFail = true;
	
	if(APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor<ASpaceShip>(ShipClass, SpawnLocation, Rotator, Parm)))
	{
		NewPlayer->Possess(PlayerPawn);
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


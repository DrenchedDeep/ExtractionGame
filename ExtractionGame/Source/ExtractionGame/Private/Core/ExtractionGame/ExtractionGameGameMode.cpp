// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ExtractionGame/ExtractionGameGameMode.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
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

void AExtractionGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}

/*
void AExtractionGameGameMode::SpawnPlayer(APlayerController* PlayerController) const
{
	APlayerStart* SpawnPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	if(!SpawnPoint)
	{
		SpawnPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	}
	
	APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor(Player, &SpawnPoint->GetActorTransform()));

	if(PlayerPawn)
	{
		PlayerController->Possess(PlayerPawn);
	}
}
*/


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

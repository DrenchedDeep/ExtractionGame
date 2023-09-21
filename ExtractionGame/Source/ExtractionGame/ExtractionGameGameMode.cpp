// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtractionGameGameMode.h"

#include "ExtractionGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

AExtractionGameGameMode::AExtractionGameGameMode()
	: Super()
{
	//bUseSeamlessTravel = true;
}

void AExtractionGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GLog->Log("Hello");
	
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

			if(Session->RegisterPlayer(FName("Session1"), *UniqueNetId, false))
			{
				GLog->Log("Succesfully registered");
			}
		}
	}
}

void AExtractionGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(!SessionCreated)
	{
		UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

		GameInstance->CreateSession(4);
		
		SessionCreated = true;
	}
}

void AExtractionGameGameMode::ServerLoad()
{
	GetWorld()->ServerTravel("SessionMap");
}

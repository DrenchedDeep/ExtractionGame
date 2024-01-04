// Copyright Epic Games, Inc. All Rights Reserved.


#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/ExtractionGame/ExtractionGameGameMode.h"
#include "UI/ExtractionGameHUD.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Core/ExtractionGame/TDMGameMode.h"
#include "Core/ExtractionGame/TDMGameState.h"
#include "Camera/CameraComponent.h"
#include "Core/ExtractionGame/SpaceShip.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Net/UnrealNetwork.h"

void AExtractionGamePlayerController::ReturnToLobby()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());

	GameInstance->OnRaidOver(true, 25.f);
	GameInstance->ShowLoadingScreen();
	
	if(GameInstance->CurrentSession)
	{
		GameInstance->DestroySession();
	}

	//for now, we'll save the inventory for testing but we need to delete the saved inventory if we leave the match by choice
	if(const AExtractionGameCharacter* PlayerCharacter =  Cast<AExtractionGameCharacter>(GetPawn()))
	{
		GameInstance->BuildPlayerSessionData(PlayerCharacter->InventoryComponent->InventoryItems, TArray<FName>());
	}

	UGameplayStatics::OpenLevel(GetWorld(), "LVL_MainMenu?listen");
}

void AExtractionGamePlayerController::Client_OnDeath_Implementation(const FString& PlayerName)
{
	if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
	{
		HUD->DeathWidget->AddToViewport();
		HUD->DeathWidget->ShowScreen(PlayerName);
	}
}

void AExtractionGamePlayerController::Server_SetName_Implementation(const FString& PlayerName)
{
	PlayerState->SetPlayerName(PlayerName);
}

void AExtractionGamePlayerController::Client_Respawn_Implementation()
{
	if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
	{
		HUD->DeathWidget->RemoveFromParent();
	}
}

void AExtractionGamePlayerController::OnDeath(const FString& PlayerName)
{
	if(GetPawn())
	{
		AExtractionGameCharacter* GameCharacter = Cast<AExtractionGameCharacter>(GetPawn());
		PlayerPawnActor = GameCharacter;
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector Loc = GameCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		FRotator Rot = GameCharacter->GetFirstPersonCameraComponent()->GetComponentRotation();

	
		ASpectatorPawn* SP =  GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorPawnSubclass, Loc, Rot, SpawnParameters);
		UnPossess();
		Possess(SP);

		CurrentRespawnTimer = RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AExtractionGamePlayerController::RespawnTick, 1.f, true);
		Client_OnDeath(PlayerName);

		if(ATDMGameState* GameState = Cast<ATDMGameState>(GetWorld()->GetGameState()))
		{
			GameState->OnPlayerKilled(PlayerName, PlayerState->GetPlayerName(), "KILLED");
		}
	}
}


void AExtractionGamePlayerController::Client_OnPlayerKilled_Implementation(const FString& KillerName,
                                                                           const FString& VictimName, const FString& DeathCause)
{
	if(PlayerState->GetPlayerName() == KillerName)
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
		{
			HUD->OnKill(VictimName);
		}
	}
}

void AExtractionGamePlayerController::RespawnTick()
{
	CurrentRespawnTimer -= 1;

	if(CurrentRespawnTimer <= 0)
	{
		if(PlayerPawnActor)
		{
			PlayerPawnActor->Destroy();
		}

		if(ATDMGameMode* GameMode = Cast<ATDMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			ATDMPlayerState* PS = Cast<ATDMPlayerState>(PlayerState);
			GameMode->SpawnPlayer(this, PS->TeamID);
		}

		Client_Respawn();
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
	}
}


void AExtractionGamePlayerController::Client_ReturnToLobby_Implementation()
{
	ReturnToLobby();
}

void AExtractionGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//It now depends on what we posses... Except this never actually happens in net mode..?
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		
#if UE_EDITOR
		if(InPawn->IsA(ASpaceShip::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("\x1b[34mPossessed a MOUNT pawn\x1b[0m"))	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("\x1b[31mPossessed a PLAYER pawn\x1b[0m"))
		}
#endif
		
		Subsystem->AddMappingContext(InPawn->IsA(ASpaceShip::StaticClass())? MountControllerMapping : PlayerControllerMapping, 0);
	}

	OnRep_PlayerState();
}

void AExtractionGamePlayerController::ClientWasKicked_Implementation(const FText& KickReason)
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->ShowLoadingScreen();
	
	if(GameInstance->CurrentSession)
	{
		GameInstance->DestroySession();
	}
	UGameplayStatics::OpenLevel(GetWorld(), "LVL_MainMenu?listen");
}

void AExtractionGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGamePlayerController, CurrentRespawnTimer);
}

void AExtractionGamePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());

	Server_SetName(GameInstance->GetPlayerUsername());

	APawn* possessed = GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("CONTROLLER: Possessed a: %s"), *possessed->GetName())

	if(const AExtractionGameCharacter* character = Cast<AExtractionGameCharacter>(possessed))
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
		{
			HUD->CreatePlayerBarDataWidget();
		
			UE_LOG(LogTemp, Warning, TEXT("PLAYER HUD BUILT"))
			character->InitializeUIComponents(HUD);
		}
	}

}




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
#include "Components/PlayerInventoryComponent.h"
#include "Core/ExtractionGame/ExtractionGameState.h"
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
		GameInstance->BuildPlayerSessionData(PlayerCharacter->InventoryComponent->GetPlayerInventory(), PlayerCharacter->InventoryComponent->GetGemInventory());
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

void AExtractionGamePlayerController::Client_SpawnItems_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Client spawning items"));
	if(const AExtractionGameState* GameState = Cast<AExtractionGameState>(GetWorld()->GetGameState()))
	{
//		GameState->GetItemReplicationManager()->SpawnItems();
	}
}

void AExtractionGamePlayerController::Server_PickupItem_Implementation(AItemActor* ItemActor)
{
	if(ItemActor)
	{
		ItemActor->Destroy();
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
		UE_LOG(LogTemp, Warning, TEXT("RESPAWNING PLAYER"))
		if(ATDMGameMode* GameMode = Cast<ATDMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			ATDMPlayerState* PS = Cast<ATDMPlayerState>(PlayerState);
			GameMode->SpawnPlayer(this, PS->TeamID);
			UE_LOG(LogTemp, Warning, TEXT("TDM RESPAWN"))

		}
		else if(const AExtractionGameGameMode* ExtractionGameGameMode = Cast<AExtractionGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			ExtractionGameGameMode->RespawnShip(this,0);
			UE_LOG(LogTemp, Warning, TEXT("EXTRACTION RESPAWN"))
		}
		
		Client_Respawn();
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("RESPAWNING COMPLETE"))

	}
}

float AExtractionGamePlayerController::GetServerWorldTimeDelta() const
{
	return ServerWorldTimeDelta;
}

float AExtractionGamePlayerController::GetServerWorldTime() const
{
	return GetWorld()->GetTimeSeconds() + ServerWorldTimeDelta;
}

void AExtractionGamePlayerController::PostNetInit()
{
	Super::PostNetInit();

	if(GetLocalRole() != ROLE_Authority)
	{
		RequestWorldTime_Internal();

		if(NetworkClockUpdateFrequency > 0.f)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle,
				this, &AExtractionGamePlayerController::RequestWorldTime_Internal, NetworkClockUpdateFrequency);
		}
	}
}

void AExtractionGamePlayerController::RequestWorldTime_Internal()
{
	ServerRequestWorldTime(GetWorld()->DeltaTimeSeconds);
}

void AExtractionGamePlayerController::ServerRequestWorldTime_Implementation(float ClientTimestamp)
{
	const float Timestamp = GetWorld()->DeltaTimeSeconds;
	ClientUpdateWorldTime(ClientTimestamp, Timestamp);
}

void AExtractionGamePlayerController::ClientUpdateWorldTime_Implementation(float ClientTimestamp, float ServerTimestamp)
{
	const float RTT = GetWorld()->DeltaTimeSeconds - ClientTimestamp;

	RTTCircularBuffer.Add(RTT);
	float AdjustedRTT = 0;
	if (RTTCircularBuffer.Num() == 10)
	{
		TArray<float> tmp = RTTCircularBuffer;
		tmp.Sort();
		for (int i = 1; i < 9; ++i)
		{
			AdjustedRTT += tmp[i];
		}
		AdjustedRTT /= 8;
		RTTCircularBuffer.RemoveAt(0);
	}
	else
	{
		AdjustedRTT = RTT;
	}
	
	ServerWorldTimeDelta = ServerTimestamp - ClientTimestamp - AdjustedRTT / 2.f;
}


void AExtractionGamePlayerController::Client_ReturnToLobby_Implementation()
{
	ReturnToLobby();
}

//This is a server side only function.
void AExtractionGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//This is likely being called more than it needs to be, I changed it too, so hopefully it still works.
	//UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());
	//Server_SetName(GameInstance->GetPlayerUsername());
	
	//It now depends on what we posses... Except this never actually happens in net mode..?
	
	UE_LOG(LogTemp, Warning, TEXT("On Possessed by Server: %hs"), (HasAuthority()?"true":"false"))
	OnRep_Pawn();
	//OnRep_PlayerState(); Why tf is is this here? -- Because if you are on standalone, or play as server.. These onReps are not needed in final?
	
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

//TODO delete
void AExtractionGamePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp,Warning,TEXT("OnRep_PlayerState, AUTH: %hhd"), HasAuthority())
	//APawn* possessed = GetPawn();
	//if(!possessed) return;
}

void AExtractionGamePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	UE_LOG(LogTemp,Warning,TEXT("OnRep_Pawn, AUTH: %hhd"), HasAuthority())

	APawn* NewPawn = GetPawn();
	if(!NewPawn) return;
	UE_LOG(LogTemp,Warning,TEXT("OnRep_Pawn, NAME: %s"), *NewPawn->GetName())
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		
#if UE_EDITOR
		if(NewPawn->IsA(ASpaceShip::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Possessed a MOUNT pawn"))	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Possessed a PLAYER pawn"))
		}
#endif
		Subsystem->AddMappingContext(NewPawn->IsA(ASpaceShip::StaticClass())? MountControllerMapping : PlayerControllerMapping, 0);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("CONTROLLER: Possessed a: %s, is Server %hs"), *NewPawn->GetName(), (HasAuthority()?"true":"false"))

	if(const AExtractionGameCharacter* character = Cast<AExtractionGameCharacter>(NewPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Check Succeeded"))
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
		{
			HUD->CreatePlayerBarDataWidget();
		
			UE_LOG(LogTemp, Warning, TEXT("PLAYER HUD BUILT"))
			character->InitializeUIComponents(HUD);
		}
	}
}

void AExtractionGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(!HasAuthority()) return;
	
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());
	Server_SetName(GameInstance->GetPlayerUsername());
	//Server_SetName(PlayerState->GetPlayerName());
	UE_LOG(LogTemp,Warning,TEXT("Binding events: %s"), *GameInstance->GetPlayerUsername());
}





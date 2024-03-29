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
#include "Camera/CameraActor.h"
#include "Net/UnrealNetwork.h"

void AExtractionGamePlayerController::ReturnToLobby()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());
	const AExtractionGamePlayerState* PS = Cast<AExtractionGamePlayerState>(PlayerState);

	if(!PS || !GameInstance)
	{
		return;
	}
	
	const float EndPlayerTime = GetWorld()->GetTimeSeconds() - StartPlayTime;

	GameInstance->OnRaidOver(RespawnsLeft > 0, EndPlayerTime, PS->GetEssence());
	GameInstance->ShowLoadingScreen();
	
	if(GameInstance->CurrentSession)
	{
		GameInstance->DestroySession();
	}

	if(const AExtractionGameCharacter* PlayerCharacter =  Cast<AExtractionGameCharacter>(GetPawn()))
	{
		GameInstance->BuildPlayerSessionData(PlayerCharacter->InventoryComponent->GetPlayerInventory(),
			PlayerCharacter->InventoryComponent->GetPlayerInventory(),
			PlayerCharacter->InventoryComponent->GetGemInventory());
	}


	UGameplayStatics::OpenLevel(GetWorld(), "LVL_MainMenu");
}

TArray<APawn*> AExtractionGamePlayerController::GetPartyMemberPawns()
{
	AExtractionGameState* GameState = GetWorld()->GetGameState<AExtractionGameState>();
	TArray<APawn*> PartyMembers;

	if(GameState)
	{
		FReplicatedPartyInfo PartyInfo = GameState->GetPartyByID(PartyID);
		for(APlayerState* PS : PartyInfo.PartyMembers)
		{
			if(PS && PS->GetPawn())
			{
				PartyMembers.Add(PS->GetPawn());
			}
		}
	}

	return PartyMembers;
}

void AExtractionGamePlayerController::Server_SendPartyInfo_Implementation(FPartyInfo Party)
{
	if(AExtractionGameGameMode* GameMode = Cast<AExtractionGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->OnPartyInfoRecieved(this, Party);
		PartyID = Party.PartyID;
	}
}

void AExtractionGamePlayerController::Client_OnDeath_Implementation(const FString& PlayerName)
{
	if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
	{
		HUD->DeathWidget->AddToViewport();
		HUD->GetPlayerBarWidget()->RemoveFromParent();
		HUD->DeathWidget->ShowScreen(PlayerName);
		HUD->ToggleCompass(false);
		HUD->ToggleStats(false);
	}

	UExtractionGameInstance* inst = Cast<UExtractionGameInstance>(GetGameInstance());
	if(!inst || !inst->SaveData || inst->SaveData->GetTutorialDeath()) return;
	inst->SaveData->SetTutorialDeath(true);
	inst->TutorialFirstDeath();
	
}

void AExtractionGamePlayerController::Server_SetName_Implementation(const FString& PlayerName)
{
	PlayerState->SetPlayerName(PlayerName);
}

void AExtractionGamePlayerController::Client_Respawn_Implementation(bool bOut)
{
	AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD());

	HUD->DeathWidget->RemoveFromParent();
	HUD->GetPlayerBarWidget()->RemoveFromParent();

	if(bOut)
	{
		HUD->OnGameOver(true);
	}
	else
	{
		HUD->ToggleRespawnWidget(true);
	}
	
}


void AExtractionGamePlayerController::Server_PickupItem_Implementation(AItemActor* ItemActor)
{
	if(ItemActor)
	{
		ItemActor->Destroy();
	}
}

void AExtractionGamePlayerController::Client_EnteredExtractionBeacon_Implementation(AExtractionBeacon* Beacon)
{
	if(Beacon->GetIsExtracting())
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
		{
			HUD->ExtractionWidget->AddToViewport();
		}

		OnEnteredExtractionBeacon(Beacon);
	}
}

void AExtractionGamePlayerController::Client_LeftExtractionBeacon_Implementation()
{
	if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
	{
		HUD->ExtractionWidget->RemoveFromParent();
	}

	OnLeftExtractionBeacon();
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

		AExtractionGameGameMode* GameMode = Cast<AExtractionGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if(GameMode && GameMode->bEnableRespawn)
		{
			CurrentRespawnTimer = RespawnTimer;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AExtractionGamePlayerController::RespawnTick, 1.f, true);
		}

		Client_OnDeath(PlayerName);
		
		if(AExtractionGameState* GameState = Cast<AExtractionGameState>(GetWorld()->GetGameState()))
		{
			GameState->OnPlayerKilled(PlayerName, PlayerState->GetPlayerName(), "KILLED");
		}
	}
}

void AExtractionGamePlayerController::RespawnPressed()
{
	FVector SpawnLoc;
	FRotator SpawnRot;

	if(AExtractionGameState* GS = Cast<AExtractionGameState>(GetWorld()->GetGameState()))
	{
		GS->GetSpaceShipSpawnInfo(SpawnLoc, SpawnRot);
	}

	OnRespawnPressed(SpawnLoc, SpawnRot);

	FTimerDelegate RespawnDelegate =
		FTimerDelegate::CreateUObject( this, &AExtractionGamePlayerController::SendSpawnRequestToServer, SpawnLoc, SpawnRot );

	GetWorld()->GetTimerManager().SetTimer(DelaySendRequestToSpawnServer, RespawnDelegate, 1.5f, false);


	if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
	{
		HUD->ToggleRespawnWidget(false);
	}
}


 void AExtractionGamePlayerController::Server_RespawnPressed_Implementation(FVector Location, FRotator Rotation)
{
	if(const AExtractionGameGameMode* ExtractionGameGameMode
		= Cast<AExtractionGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		ExtractionGameGameMode->RespawnShip(this,0, Location, Rotation);
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
		if(RespawnsLeft <= 0)
		{
			//kick player
			RespawnsLeft = 0;
		}


		Client_Respawn(RespawnsLeft == 0);


		if(PlayerPawnActor)
		{
			PlayerPawnActor->Destroy();
		}

		RespawnsLeft--;

		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	}
}

void AExtractionGamePlayerController::SendSpawnRequestToServer(FVector InSpawnLocation, FRotator InSpawnRotator)
{
	Server_RespawnPressed(InSpawnLocation, InSpawnRotator);
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

	//stupid listen server stuff
	if(IsLocalController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();

			if(InPawn->IsA(ASpaceShip::StaticClass()))
			{
				Subsystem->AddMappingContext(MountControllerMapping, 0);
			}
			else
			{
				Subsystem->AddMappingContext(PlayerControllerMapping, 0);
				OnRep_Pawn();
			}
			
		}
	}
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
	DOREPLIFETIME(AExtractionGamePlayerController, RespawnsLeft);
}

//TODO delete
void AExtractionGamePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//APawn* possessed = GetPawn();
	//if(!possessed) return;
	
}

void AExtractionGamePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	APawn* NewPawn = GetPawn();
	if(!NewPawn) return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		
		Subsystem->AddMappingContext(NewPawn->IsA(ASpaceShip::StaticClass())? MountControllerMapping : PlayerControllerMapping, 0);
	}

	if(AExtractionGameState* GS = Cast<AExtractionGameState>(GetWorld()->GetGameState()))
	{
		if(GS->GetExtractionGameState() == EGameModeState::Playing)
		{
			if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
			{
				HUD->ToggleCompass(true);
				HUD->ToggleStats(true);
			}
		}
	}

	if(const AExtractionGameCharacter* character = Cast<AExtractionGameCharacter>(NewPawn))
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(GetHUD()))
		{
			HUD->CreatePlayerBarDataWidget();
			HUD->GetPlayerBarWidget()->AddToViewport();
		
			character->InitializeUIComponents(HUD);
		}
	}
}

void AExtractionGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	RespawnsLeft = 3;
	if(HasAuthority())
	{
		UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetWorld()->GetGameInstance());
		Server_SetName(GameInstance->GetPlayerUsername());
		//Server_SetName(PlayerState->GetPlayerName());
	}
	
	if(IsLocalController())
	{
		FPartyInfo PartyInfo = Cast<UExtractionGameInstance>(GetGameInstance())->PartyInfo;
		StartPlayTime = GetWorld()->GetTimeSeconds();

		if(PartyInfo.bIsValid)
		{
			Server_SendPartyInfo(PartyInfo);
			PartyID = PartyInfo.PartyID;
		}
	}

	RespawnManager = Cast<ARespawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), RespawnManagerSubclass));
}

void AExtractionGamePlayerController::Server_StartExtraction_Implementation(AExtractionBeacon* Beacon)
{
	if(Beacon)
	{
		Beacon->StartExtraction();
	}
}





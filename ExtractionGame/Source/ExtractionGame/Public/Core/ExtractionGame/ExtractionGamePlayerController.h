// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RespawnManager.h"
#include "RespawnStashManager.h"
#include "Core/Managers/PlayerStashManager.h"
#include "Extraction/ExtractionBeacon.h"
#include "GameFramework/PlayerController.h"
#include "Items/ItemActor.h"
#include "ExtractionGamePlayerController.generated.h"

struct FPartyInfo;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class EXTRACTIONGAME_API AExtractionGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpectatorPawn> SpectatorPawnSubclass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARespawnManager> RespawnManagerSubclass;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTimer = 5.f;
	UPROPERTY(EditDefaultsOnly)
	float RespawnDelay = 1.5f;
	
	FTimerHandle RespawnTimerHandle;

	UPROPERTY()
	AActor* PlayerPawnActor;

	
public:
	UFUNCTION(BlueprintCallable)
	virtual void ReturnToLobby();
	UFUNCTION(BlueprintCallable)
	TArray<APawn*> GetPartyMemberPawns();
	
	UFUNCTION(Reliable, Client)
	void Client_ReturnToLobby();
	UFUNCTION(Reliable, Client)
	virtual void Client_OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);
	UFUNCTION(Server, Reliable)
	void Server_PickupItem(AItemActor* ItemActor);
	UFUNCTION(Server, Reliable)
	void Server_SendPartyInfo(FPartyInfo Party);
	UFUNCTION(Unreliable, Client)
	void Client_EnteredExtractionBeacon(AExtractionBeacon* Beacon);
	UFUNCTION(Unreliable, Client)
	void Client_LeftExtractionBeacon();

	virtual void OnDeath(const FString& PlayerName);
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float CurrentRespawnTimer;

	UPROPERTY(BlueprintReadOnly)
	int32 PartyID;
	UPROPERTY(BlueprintReadWrite)
	ARespawnStashManager* RespawnManager;

	bool bInitStartedItems = false;

	UFUNCTION(BlueprintCallable)
	void RespawnPressed();

	UFUNCTION(BlueprintCallable)
	int32 GetRespawnsLeft() const { return RespawnsLeft; }
private:
	UPROPERTY(Replicated)
	int32 RespawnsLeft;

	UFUNCTION(Server, Reliable)
	void Server_RespawnPressed(FVector Location, FRotator Rotation);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* PlayerControllerMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* MountControllerMapping;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void ClientWasKicked_Implementation(const FText& KickReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Pawn() override;
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_StartExtraction(AExtractionBeacon* Beacon);
	UFUNCTION(Reliable, Client)
	void Client_OnDeath(const FString& PlayerName);
	UFUNCTION(Reliable, Client)
	void Client_Respawn(bool bOver);
	UFUNCTION(Reliable, Server)
	void Server_SetName(const FString& PlayerName);
	
	
	virtual void RespawnTick();


	UFUNCTION(BlueprintImplementableEvent)
	void OnEnteredExtractionBeacon(AExtractionBeacon* Beacon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLeftExtractionBeacon();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRespawnPressed(FVector OutSpawnLocation, FRotator OutSpawnRotator);

	FTimerHandle DelaySendRequestToSpawnServer;
	UFUNCTION()
	void SendSpawnRequestToServer(FVector InSpawnLocation, FRotator InSpawnRotator);
//network clock
private:
	float ServerWorldTimeDelta = 0.f;
	TArray<float> RTTCircularBuffer;
protected:

	UPROPERTY(EditDefaultsOnly, Category=GameState)
	float NetworkClockUpdateFrequency = 5.f;

public:
	UFUNCTION(BlueprintPure)
	float GetServerWorldTimeDelta() const;

	UFUNCTION(BlueprintPure)
	float GetServerWorldTime() const;

	void PostNetInit() override;

private:
	void RequestWorldTime_Internal();
	
	UFUNCTION(Server, Unreliable)
	void ServerRequestWorldTime(float ClientTimestamp);
	
	UFUNCTION(Client, Unreliable)
	void ClientUpdateWorldTime(float ClientTimestamp, float ServerTimestamp);

	FVector ClientSpawnLocation;
	float StartPlayTime;
};


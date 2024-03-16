// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	float RespawnTimer = 5.f;
	
	
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
	//reason i put it here is cuz i think its more reliable and cleaner to use client rpcs for important things like spawning items
	UFUNCTION(Reliable, Client)
	void Client_SpawnItems();

	virtual void OnDeath(const FString& PlayerName);
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float CurrentRespawnTimer;

	UPROPERTY(BlueprintReadOnly)
	int32 PartyID;

	bool bInitStartedItems = false;

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
	void Client_Respawn();
	UFUNCTION(Reliable, Server)
	void Server_SetName(const FString& PlayerName);
	
	virtual void RespawnTick();


	UFUNCTION(BlueprintImplementableEvent)
	void OnEnteredExtractionBeacon(AExtractionBeacon* Beacon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLeftExtractionBeacon();

	
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


	float StartPlayTime;
};

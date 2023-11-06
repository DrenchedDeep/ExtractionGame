// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExtractionGamePlayerController.generated.h"

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

	AActor* PlayerPawnActor;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void ReturnToLobby();
	UFUNCTION(Reliable, Client)
	void Client_ReturnToLobby();
	virtual void OnDeath(const FName& PlayerName);
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float CurrentRespawnTimer;

	virtual void OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);
	
protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void ClientWasKicked_Implementation(const FText& KickReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(Reliable, Client)
	void Client_OnDeath(const FName& PlayerName);
	UFUNCTION(Reliable, Client)
	void Client_Respawn();
	UFUNCTION(Reliable, Server)
	void Server_SetName(const FString& PlayerName);

	UFUNCTION(Reliable, Client)
	virtual void Client_OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);

	virtual void RespawnTick();
};

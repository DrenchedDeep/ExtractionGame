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

	UPROPERTY()
	AActor* PlayerPawnActor;

	

public:
	UFUNCTION(BlueprintCallable)
	virtual void ReturnToLobby();
	UFUNCTION(Reliable, Client)
	void Client_ReturnToLobby();
	UFUNCTION(Reliable, Client)
	virtual void Client_OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);

	virtual void OnDeath(const FString& PlayerName);
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float CurrentRespawnTimer;


	
protected:
	/** Input Mapping Context to be used for player input */
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
	

	UFUNCTION(Reliable, Client)
	void Client_OnDeath(const FString& PlayerName);
	UFUNCTION(Reliable, Client)
	void Client_Respawn();
	UFUNCTION(Reliable, Server)
	void Server_SetName(const FString& PlayerName);
	
	virtual void RespawnTick();
};

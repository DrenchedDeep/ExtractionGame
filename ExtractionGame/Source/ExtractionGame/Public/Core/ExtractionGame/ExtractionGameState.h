

#pragma once

#include "CoreMinimal.h"
#include "EndMatchController.h"
#include "AI/GooberSoundsManager.h"
#include "GameFramework/GameStateBase.h"
#include "Managers/ItemReplicationManager.h"
#include "ExtractionGameGameMode.h"
#include "ExtractionGameState.generated.h"

USTRUCT(BlueprintType)
struct FReplicatedPartyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerState*> PartyMembers;
	UPROPERTY(BlueprintReadOnly)
	int32 PartyID;
};

UCLASS()
class EXTRACTIONGAME_API AExtractionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGooberSoundsManager* GooberSounds;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGooberSoundsManager> GooberSoundSubclass;

public:
	AExtractionGameState();

	void OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);

	UFUNCTION(BlueprintCallable)
	TArray<FReplicatedPartyInfo> GetReplicatedParties() const { return ReplicatedParties; }
	
	UFUNCTION(BlueprintCallable)
	FReplicatedPartyInfo GetPartyByID(int32 PartyID);

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EGameModeState> GetExtractionGameState() const { return ExtractionGameState; }

	UFUNCTION(BlueprintCallable)
	TArray<AExtractionGamePlayerState*> GetTopThreePlayers() const { return TopThreePlayers; }
	
	UFUNCTION(BlueprintCallable)
	float GetMatchTimer() const { return MatchTimer; }

	UFUNCTION(BlueprintCallable)
	void AddGoobersKilled(int32 GoobersKilledToAdd) { GoobersKilled += GoobersKilledToAdd; }

	void UpdateTotalPlayerKills() { TotalPlayerKills++; }

	UFUNCTION(BlueprintCallable)
	int32 GetGoobersKilled() const { return GoobersKilled; }
	UFUNCTION(BlueprintCallable)
	int32 GetTotalPlayerKills() const { return TotalPlayerKills; }
	
	void UpdateParties(TArray<FInGameParty> Parties);
	void SetMatchTimer(int32 NewMatchTimer) { MatchTimer = NewMatchTimer; OnRep_MatchTimer(); }
	void SetState(TEnumAsByte<EGameModeState> NewState);
	void SetTopThreePlayers(TArray<AExtractionGamePlayerState*> NewTopThreePlayers);
	void SetBlockMovement(bool bInBlockMovement);

	void GetSpaceShipSpawnInfo(FVector& OutLocation, FRotator& OutRotation);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_ExtractionGameState();
	UFUNCTION()
	virtual void OnRep_TopThreePlayers();
	UFUNCTION()
	virtual void OnRep_BlockMovement();
	UFUNCTION(BlueprintImplementableEvent)
	void OnExtractionGameStateUpdated();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTopThreePlayersUpdated();

	bool warningVoiceline;

	UPROPERTY(EditDefaultsOnly)int32 RemainingTimeWarning;
	UFUNCTION(BlueprintImplementableEvent) void GameNearlyOverBoys();
	
	UFUNCTION()
	void OnRep_MatchTimer();
	
private:
	UPROPERTY(Replicated)
	TArray<FReplicatedPartyInfo> ReplicatedParties;
	UPROPERTY(ReplicatedUsing = OnRep_MatchTimer)
	int32 MatchTimer;
	UPROPERTY(Replicated)
	int32 GoobersKilled;
	UPROPERTY(Replicated)
	int32 TotalPlayerKills;
	
	UPROPERTY(ReplicatedUsing=OnRep_BlockMovement)
	bool bBlockMovement;
	
	UPROPERTY(ReplicatedUsing=OnRep_TopThreePlayers)
	TArray<AExtractionGamePlayerState*> TopThreePlayers;

	UPROPERTY(ReplicatedUsing=OnRep_ExtractionGameState)
	TEnumAsByte<EGameModeState> ExtractionGameState;
};

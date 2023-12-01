

#pragma once

#include "CoreMinimal.h"
#include "Spawnpoint.h"
#include "TDMPlayerState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerStart.h"
#include "TDMGameState.generated.h"

USTRUCT(BlueprintType)
struct FTDMTeam
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	uint8 TeamID;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Score;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ATDMPlayerState*> PlayerStates;

	FTDMTeam(uint8 TeamID)
		: TeamID(TeamID), Score(0)
	{
	}

	FTDMTeam() = default;
};


UCLASS()
class EXTRACTIONGAME_API ATDMGameState : public AGameState
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MatchLength = 30.f;
	
	
public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	FTDMTeam RedTeam;
	UPROPERTY(BlueprintReadOnly, Replicated)
	FTDMTeam BlueTeam;

	UPROPERTY(ReplicatedUsing=OnRep_WinningTeamID)
	uint8 WinningTeamID;
	UPROPERTY(ReplicatedUsing=OnRep_MatchTime)
	float MatchTime;
	UPROPERTY(ReplicatedUsing=OnRep_CanInteract)
	bool bCanInteract;


	TArray<ASpawnpoint*> RedTeamDefaultsSpawnpoints;
	TArray<ASpawnpoint*> BlueTeamDefaultsSpawnpoints;
	
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	virtual int32 RegisterPlayerToTeam(APlayerController* PlayerController);

	void OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_MatchState() override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleLeavingMap() override;
	virtual void OnRep_ElapsedTime() override;
	virtual void DefaultTimer() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void MatchStateChanged(const FName& NewState);

	UFUNCTION()
	void OnRep_WinningTeamID();
	UFUNCTION()
	void OnRep_MatchTime();
	UFUNCTION()
	void OnRep_CanInteract();
private:
	void EndGame();
	
	FORCEINLINE uint8 GetBestTeam() const
	{
		uint8 BestTeam = 0;

		if(RedTeam.PlayerStates.Num() < BlueTeam.PlayerStates.Num())
		{
			BestTeam = 1;
		}

		return BestTeam;
	}

	FORCEINLINE ATDMPlayerState* GetPlayerStateByName(FString PlayerName)
	{
		ATDMPlayerState* PlayerState = nullptr;

		for(int i = 0; i < PlayerArray.Num(); i++)
		{
			if(PlayerName == PlayerArray[i]->GetPlayerName())
			{
				PlayerState = Cast<ATDMPlayerState>(PlayerArray[i]);
			}
		}

		return PlayerState;
	}

public:
	FORCEINLINE APlayerStart* GetTeamSpawnPoint(uint8 TeamID) const
	{
		TArray<ASpawnpoint*> SpawnPoints = TeamID == 0 ? BlueTeamDefaultsSpawnpoints : RedTeamDefaultsSpawnpoints;

		for(int i = 0; i < SpawnPoints.Num(); i++)
		{
			if(!SpawnPoints[i]->CanSpawn())
			{
				continue;
			}
			
			return SpawnPoints[i];
		}

		return nullptr;
	}
};

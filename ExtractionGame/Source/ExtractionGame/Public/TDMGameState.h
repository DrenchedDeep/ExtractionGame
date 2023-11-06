

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

	UPROPERTY()
	uint8 TeamID;
	
	UPROPERTY()
	int32 Score;
	
	UPROPERTY()
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

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	FTDMTeam RedTeam;
	UPROPERTY(BlueprintReadOnly, Replicated)
	FTDMTeam BlueTeam;

	TArray<ASpawnpoint*> RedTeamDefaultsSpawnpoints;
	TArray<ASpawnpoint*> BlueTeamDefaultsSpawnpoints;
	
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	virtual int32 RegisterPlayerToTeam(APlayerController* PlayerController);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_MatchState() override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleLeavingMap() override;

	
	UFUNCTION(BlueprintImplementableEvent)
	void MatchStateChanged(const FName& NewState);
	
private:
	FORCEINLINE uint8 GetBestTeam() const
	{
		uint8 BestTeam = 0;

		if(RedTeam.PlayerStates.Num() < BlueTeam.PlayerStates.Num())
		{
			BestTeam = 1;
		}

		return BestTeam;
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

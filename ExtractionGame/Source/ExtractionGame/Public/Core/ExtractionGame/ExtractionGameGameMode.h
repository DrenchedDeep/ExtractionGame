#pragma once

#include "CoreMinimal.h"
#include "EndMatchController.h"
#include "ExtractionGameCharacter.h"
#include "ExtractionGameInstance.h"
#include "SpaceShip.h"
#include "GameFramework/GameModeBase.h"
#include "ExtractionGameGameMode.generated.h"

USTRUCT()
struct FInGameParty
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<APlayerController*> Players;
	UPROPERTY()
	int32 PartyID;
	UPROPERTY()
	int32 ExpectedPlayerCount;
	UPROPERTY()
	bool bAllPlayersConnected;

	FInGameParty(const TArray<APlayerController*>& Players, int32 PartyID, int32 ExpectedPlayerCount,
		bool bAllPlayersConnected)
		: Players(Players),
		  PartyID(PartyID),
		  ExpectedPlayerCount(ExpectedPlayerCount),
		  bAllPlayersConnected(bAllPlayersConnected)
	{
	}

	FInGameParty() = default;
};

UENUM(BlueprintType)
enum EGameModeState
{
	WaitingForPlayers,
	Playing,
	EndingGame,
	PostEndingGame
};

UENUM(BlueprintType)
enum EEndGameState
{
	ShowingOutcome,
	UpToUser,
};

UCLASS(minimalapi)
class AExtractionGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpaceShip> ShipClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AExtractionGameCharacter> PlayerClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEndMatchController> EndMatchControllerClass;
	
	UPROPERTY(EditDefaultsOnly)
	bool bBlockMovementTillMatchReady = true;
	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayersBeforeStarting = 4;
	
	UPROPERTY(EditDefaultsOnly)int32 MatchLength = 300;
	UPROPERTY(EditDefaultsOnly)int32 MatchShortTimeIndicator = 30;
	
	UPROPERTY(EditDefaultsOnly)
	int32 TimeBeforeEndGame = 5;
	bool SessionCreated;


public:
	AExtractionGameGameMode();

	UPROPERTY(EditDefaultsOnly)
	bool bEnableRespawn = false;
	

	void RespawnShip(APlayerController* NewPlayer, int32 TeamID) const;
	void SpawnShip(APlayerController* NewPlayer, const FVector StartLocation, const FRotator Rotator, int32 I);
	void GetPartySpawnLocation(FVector& OutLocation, FRotator& OutRotation) const;
	
	void OnPartyInfoRecieved(APlayerController* Sender, FPartyInfo PartyInfo);

	TEnumAsByte<EGameModeState>	GetGameModeState() const { return GameModeState; }
	void SetGameModeState(EGameModeState NewState);
	
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	bool bAllExistingPlayersRegistered;
	
	virtual void RegisterPlayerEOS(APlayerController* NewPlayer);
	virtual bool AllPlayersReady();
	virtual void EndGame();

	UFUNCTION(BlueprintImplementableEvent)
	void LowMatchTime();

	
	UFUNCTION()
	void CheckToStartMatch();
	UFUNCTION()
	void TickMatch();
	UFUNCTION()
	void EndGameTimer();
private:
	TArray<FInGameParty> AllParties;
	int32 SpawnedSpaceships;
	
	EGameModeState GameModeState;
	EEndGameState EndGameState;
	bool HasParty(int32 InID, int32& OutIndex);


	FTimerHandle CheckToStartMatchTimerHandle;
	FTimerHandle MatchTimerHandle;
	FTimerHandle EndGameTimerHandle;

	UPROPERTY()
	TArray<AExtractionGamePlayerState*> TopThreePlayers;
	UPROPERTY()
	AEndMatchController* EndMatchController;
};





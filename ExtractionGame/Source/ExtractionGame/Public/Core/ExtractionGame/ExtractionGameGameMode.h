#pragma once

#include "CoreMinimal.h"
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

UCLASS(minimalapi)
class AExtractionGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool bSkipShipSequence;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpaceShip> ShipClass;
	bool SessionCreated;

public:
	AExtractionGameGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	void RespawnShip(APlayerController* NewPlayer, int32 TeamID) const;
	void SpawnShip(APlayerController* NewPlayer, const FVector StartLocation, const FRotator Rotator, int32 I);
	void GetPartySpawnLocation(FVector& OutLocation, FRotator& OutRotation) const;
	
	void OnPartyInfoRecieved(APlayerController* Sender, FPartyInfo PartyInfo);
	
protected:
	bool bAllExistingPlayersRegistered;
	virtual void RegisterPlayerEOS(APlayerController* NewPlayer);

private:
	TArray<FInGameParty> AllParties;

	bool HasParty(int32 InID, int32& OutIndex);
};





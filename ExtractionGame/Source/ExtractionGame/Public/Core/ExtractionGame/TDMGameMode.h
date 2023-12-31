

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGameGameMode.h"
#include "GameFramework/GameMode.h"
#include "TDMGameMode.generated.h"

/**
 * 
 */


//derive from the base gamemode so dont have to copy and paste registering users on eos
UCLASS()
class EXTRACTIONGAME_API ATDMGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AExtractionGameCharacter> Player;

	void RespawnAllPlayerPawns();

public:
	virtual void SpawnPlayer(APlayerController* NewPlayer, int32 TeamID);
	virtual void KickAllPlayers();

private:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void KickAllPlayersTimer();
	FTimerHandle KickEveryoneTimerHandle;
};

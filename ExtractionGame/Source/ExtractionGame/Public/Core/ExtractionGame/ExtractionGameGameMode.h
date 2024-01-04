#pragma once

#include "CoreMinimal.h"
#include "ExtractionGameCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "ExtractionGameGameMode.generated.h"

UCLASS(minimalapi)
class AExtractionGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Player", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AExtractionGameCharacter> PlayerActor;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AExtractionGameCharacter> Player;

	bool SessionCreated;

	FTimerHandle TimeHandle;
	
public:
	AExtractionGameGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	void SpawnPlayer(APlayerController* PlayerController) const;
protected:
	bool bAllExistingPlayersRegistered;

	virtual void RegisterPlayerEOS(APlayerController* NewPlayer);
};




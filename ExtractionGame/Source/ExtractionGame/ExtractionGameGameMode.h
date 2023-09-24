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

	bool SessionCreated;

	FTimerHandle TimeHandle;
	
public:
	AExtractionGameGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	void RegisterPlayerEOS(APlayerController* NewPlayer);
	
private:
	bool bAllExistingPlayersRegistered;
};




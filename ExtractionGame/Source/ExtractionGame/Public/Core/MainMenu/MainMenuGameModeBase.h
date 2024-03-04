

#pragma once

#include "CoreMinimal.h"
#include "MainMenuSpaceShip.h"
#include "Core/Managers/PartyManager.h"
#include "PlayerStand.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameModeBase.generated.h"


UCLASS()
class EXTRACTIONGAME_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	
	UPROPERTY()
	TArray<AMainMenuSpaceShip*> SpaceShips;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APartyManager> PartyManagerClass;

public:
	AMainMenuGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void SetupMemberModel(APlayerState* PlayerState, const FString& Username);

			
	UPROPERTY()
	APartyManager* PartyManager;
	
};

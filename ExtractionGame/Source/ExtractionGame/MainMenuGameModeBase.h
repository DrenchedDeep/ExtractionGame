

#pragma once

#include "CoreMinimal.h"
#include "PlayerStand.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameModeBase.generated.h"


UCLASS()
class EXTRACTIONGAME_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	bool IsServer;

	TArray<APlayerStand*> PlayerStands;
	
public:
	AMainMenuGameModeBase();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};

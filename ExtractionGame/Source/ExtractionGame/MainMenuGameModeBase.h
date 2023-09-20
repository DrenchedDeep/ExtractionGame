

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	bool IsServer;

public:
	AMainMenuGameModeBase();
};
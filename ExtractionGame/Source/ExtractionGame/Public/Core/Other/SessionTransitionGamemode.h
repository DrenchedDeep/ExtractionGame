

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SessionTransitionGamemode.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API ASessionTransitionGamemode : public AGameModeBase
{
	GENERATED_BODY()

	int32 PlayerCount;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};

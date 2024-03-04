

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuSpaceShip.generated.h"

UCLASS()
class EXTRACTIONGAME_API AMainMenuSpaceShip : public AActor
{
	GENERATED_BODY()
public:
	bool bHasPlayer;
	virtual void SetOwner(AActor* NewOwner) override;

protected:
	virtual void OnRep_Owner() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetupSpaceship();
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Spawnpoint.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API ASpawnpoint : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TeamID;
	
	//check if enemies r nearby or something
	bool CanSpawn() const { return true; }
};

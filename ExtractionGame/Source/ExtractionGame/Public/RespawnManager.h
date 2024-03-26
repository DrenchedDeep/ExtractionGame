

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "GameFramework/Actor.h"
#include "RespawnManager.generated.h"
 

UCLASS()
class EXTRACTIONGAME_API ARespawnManager : public AActor
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	ACameraActor* RespawnCamera; 
};


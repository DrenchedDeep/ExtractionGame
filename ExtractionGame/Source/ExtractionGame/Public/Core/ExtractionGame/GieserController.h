

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GieserController.generated.h"

/*
 * The Gieser controllers responsibility is to spawn geisers and indicate to players that it is doing so
 * it is also responsible for managing all giesers on the map, ensuring proper clean up, and balance.
 */


UCLASS()
class EXTRACTIONGAME_API AGieserController : public AActor
{
	GENERATED_BODY()

	FTimerHandle timerHandle;
	
public:	
	// Sets default values for this actor's properties
	AGieserController();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGieserSummoned();

protected:

	UPROPERTY(BlueprintReadWrite)
	float minSpawnTime;

	UPROPERTY(BlueprintReadWrite)
	float maxSpawnTime;

	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnGieser();
};

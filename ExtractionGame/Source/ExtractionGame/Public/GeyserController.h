

#pragma once

#include "CoreMinimal.h"
#include "GeyserSpawnpoint.h"
#include "GameFramework/Actor.h"
#include "GeyserController.generated.h"

UCLASS()
class EXTRACTIONGAME_API AGeyserController : public AActor
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController")
	TArray<AGeyserSpawnpoint*> GeyserSpawnpoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController")
	float GeyserSpawnDelay = 250;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController")
	float GeyserLength = 60;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle GeyserSpawnTimer;
	
	UPROPERTY()
	AGeyserSpawnpoint* SmokingGeiser;
	UPROPERTY()
	AGeyserSpawnpoint* ActiveGeyser;
	
	UFUNCTION()
	void SpawnGeyser();

	int32 GeyserSpawnCount;
};

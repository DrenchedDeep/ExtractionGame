

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController") float GeyserSpawnDelay = 250;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController") float GeyserNearlyReadyTime = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController") float GeyserLength = 60;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController") float GeyserSmokeDuration = 15;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeyserController") float GeyserActiveDuration = 30;

	UFUNCTION(BlueprintCallable)
	void DisableAll();
	
	static TArray<AGeyserSpawnpoint*> GetGeyserSpawns()
	{
		if(!SingletonInstance.IsValid()) return TArray<AGeyserSpawnpoint*>();
		return SingletonInstance->GeyserSpawnpoints;
	}


protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle GeyserSpawnTimer;
	FTimerHandle GeyserNearSpawnTimer;
	
	UPROPERTY()
	AGeyserSpawnpoint* SmokingGeiser;
	UPROPERTY()
	AGeyserSpawnpoint* ActiveGeyser;
	
	UFUNCTION()
	void SpawnGeyser();
	void NearlySpawnGeyser();
	

	int32 GeyserSpawnCount;

	// Static weak pointer to the single instance of AGeyserController
	static TWeakObjectPtr<AGeyserController> SingletonInstance;
};

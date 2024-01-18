

#pragma once

#include "CoreMinimal.h"
#include "ItemReplicationManager.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class EXTRACTIONGAME_API AItemSpawner : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	bool bSpawnOnLandscape;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, meta = (EditCondition="bSpawnOnLandscape", ExposeOnSpawn = "true"))
	float SpawnRadius;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, meta = (EditCondition="bSpawnOnLandscape", ExposeOnSpawn = "true"))
	int32 MaxSpawnAttempts;
	
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TArray<TSubclassOf<AItemActor>> SpawnableItems;


	AItemSpawner();
	void SpawnItems(UItemReplicationManager* ItemManager);

	UItemReplicationManager* GetItemReplicationManager() const { return ItemReplicationManager; }
private:
	void SpawnItemsOnLandscape();

	UPROPERTY()
	UItemReplicationManager* ItemReplicationManager;
};

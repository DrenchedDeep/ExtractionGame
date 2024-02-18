

#pragma once

#include "CoreMinimal.h"
#include "ItemReplicationManager.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnableItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsGem"))
	float MinPolish = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsGem"))
	float MaxPolish = 100;
};

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
	TArray<FSpawnableItem> SpawnableItems;


	AItemSpawner();
	void SpawnItems(AItemReplicationManager* ItemManager);

	AItemReplicationManager* GetItemReplicationManager() const { return ItemReplicationManager; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AItemActor*> GetSpawnedItems() const { return SpawnedItems; }
private:
	void SpawnItemsOnLandscape();

	UPROPERTY()
	AItemReplicationManager* ItemReplicationManager;

	UPROPERTY()
	TArray<AItemActor*> SpawnedItems;
};

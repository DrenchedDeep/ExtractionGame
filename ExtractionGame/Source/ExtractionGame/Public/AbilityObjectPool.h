

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.h"
#include "Containers/Queue.h"

#include "AbilityObjectPool.generated.h"


USTRUCT(BlueprintType)
struct FPoolStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Object Pooling")
	TSubclassOf<APooledObject> pooledSubclass;
	UPROPERTY(EditAnywhere, Category="Object Pooling")
	int numToSpawn = 20;
};



UCLASS()
class EXTRACTIONGAME_API AAbilityObjectPool : public AActor
{
	GENERATED_BODY()

public:

	//APooledObject* EmergencySpawn(const FString& map);
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Object Pooling")
	APooledObject* SpawnPoolObject(FString map);

	UPROPERTY(EditAnywhere, Category="Object Pooling")
	TArray<FPoolStruct> PooledObjectSubclass;

protected:
	virtual void BeginPlay() override;

	void OnPoolObjectDespawned(APooledObject* obj, FString pool);
	TMap<FString, int> currentIterators;
	TMap<FString,TArray<APooledObject*>> ObjectPool;
	
};

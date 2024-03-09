

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/PooledObject.h"
#include "AbilityObjectPool.generated.h"


USTRUCT(BlueprintType)
struct FPoolStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledObject> pooledSubclass;
	
	UPROPERTY(EditAnywhere)
	int numToSpawn = 20;
};

USTRUCT(BlueprintType)
struct FPoolSet
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Object Pooling")
	TArray<APooledObject*> pool;
	UPROPERTY(EditAnywhere, Category="Object Pooling")
	int index = 0;
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
	//UFUNCTION()
	//void OnPoolObjectDespawned(APooledObject* obj, FString pool);
	
	UPROPERTY(VisibleInstanceOnly)
	TMap<FString,FPoolSet> ObjectPool;
	
};

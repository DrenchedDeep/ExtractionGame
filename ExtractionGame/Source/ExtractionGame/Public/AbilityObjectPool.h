

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h" // Include this for TQueue
#include "Components/ActorComponent.h"
#include "AbilityObjectPool.generated.h"



UCLASS()
class EXTRACTIONGAME_API AAbilityObjectPool : public AActor
{
	GENERATED_BODY()

public:
	AAbilityObjectPool();
	
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledObject(TSubclassOf<AActor> ActorClass);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReturnPooledObject(AActor* PooledObject);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastReturnPooledObject(AActor* PooledObject);

	UPROPERTY(EditInstanceOnly, Category = "Object Pool")
	TArray<TSubclassOf<AActor>> PooledObjectClasses;

	UPROPERTY(EditInstanceOnly, Category = "Object Pool")
	TArray<int32> PoolSizes;

private:
	AActor* SpawnPooledObject(TSubclassOf<AActor> ActorClass);
	void ResetObjectState(AActor* PooledObject);

	TMap<TSubclassOf<AActor>, TSharedPtr<TQueue<AActor*, EQueueMode::Spsc>>> ObjectPoolMap;

	UPROPERTY(Replicated)
	TArray<AActor*> ReplicatedPooledObjects;
		
};

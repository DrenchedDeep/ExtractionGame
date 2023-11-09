// ObjectPool.cpp

#include "AbilityObjectPool.h"
#include "Net/UnrealNetwork.h"

AAbilityObjectPool::AAbilityObjectPool()
{
    for (int32 i = 0; i < PooledObjectClasses.Num(); ++i)
    {
        TSubclassOf<AActor> ActorClass = PooledObjectClasses[i];
        int32 PoolSize = (i < PoolSizes.Num()) ? PoolSizes[i] : 0;

        // Use pointer to TQueue wrapped in TSharedPtr
        TSharedPtr<TQueue<AActor*, EQueueMode::Spsc>> ActorQueuePtr = ObjectPoolMap.FindOrAdd(ActorClass);
        TQueue<AActor*, EQueueMode::Spsc>& ActorQueue = *ActorQueuePtr;

        for (int32 j = 0; j < PoolSize; ++j)
        {
            AActor* PooledObject = SpawnPooledObject(ActorClass);
            if (PooledObject)
            {
                PooledObject->SetActorHiddenInGame(true);
                ActorQueue.Enqueue(PooledObject);
            }
        }
    }
}

AActor* AAbilityObjectPool::GetPooledObject(TSubclassOf<AActor> ActorClass)
{
    if (!ObjectPoolMap.Contains(ActorClass)) return nullptr;

    TQueue<AActor*>& ActorQueue = ObjectPoolMap[ActorClass];
    if (!ActorQueue.IsEmpty())
    {
        AActor* PooledObject = nullptr;
        ActorQueue.Dequeue(PooledObject);

        // Object is available, return it
        PooledObject->SetActorHiddenInGame(false);
        MulticastReturnPooledObject(PooledObject);
        return PooledObject;
    }
    // Queue is empty, create a new object
    if (AActor* NewPooledObject = SpawnPooledObject(ActorClass))
    {
        MulticastReturnPooledObject(NewPooledObject);
        return NewPooledObject;
    }

    // No available objects or invalid actor class
    return nullptr;
}

void AAbilityObjectPool::ServerReturnPooledObject_Implementation(AActor* PooledObject)
{
    if (PooledObject)
    {
        ResetObjectState(PooledObject);
        MulticastReturnPooledObject(PooledObject);
    }
    else
    {
        // Log an error if the PooledObject is null
        UE_LOG(LogTemp, Error, TEXT("ServerReturnPooledObject: Received null PooledObject"));
    }
}

bool AAbilityObjectPool::ServerReturnPooledObject_Validate(AActor* PooledObject)
{
    return true;
}

void AAbilityObjectPool::MulticastReturnPooledObject_Implementation(AActor* PooledObject)
{
    if (PooledObject)
    {
        ResetObjectState(PooledObject);
        ReplicatedPooledObjects.Add(PooledObject);
    }
}

void AAbilityObjectPool::ResetObjectState(AActor* PooledObject)
{
    if (PooledObject)
    {
        // Reset the object's state
        //PooledObject->SetActorLocation(FVector::ZeroVector);
        //PooledObject->SetActorRotation(FRotator::ZeroRotator);
        PooledObject->SetActorHiddenInGame(true);
        PooledObject->Reset();
    }
}

AActor* AAbilityObjectPool::SpawnPooledObject(TSubclassOf<AActor> ActorClass)
{
    if (ActorClass)
    {
        if (UWorld* World = GetWorld())
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            AActor* NewObject = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
            return NewObject;
        }
        else
        {
            // Log an error if the world is invalid
            UE_LOG(LogTemp, Error, TEXT("SpawnPooledObject: Invalid World"));
        }
    }
    else
    {
        // Log an error if the actor class is invalid
        UE_LOG(LogTemp, Error, TEXT("SpawnPooledObject: Invalid ActorClass"));
    }

    return nullptr;
}

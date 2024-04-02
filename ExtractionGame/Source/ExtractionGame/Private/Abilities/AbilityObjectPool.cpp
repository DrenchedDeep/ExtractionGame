// ObjectPool.cpp

#include "Abilities/AbilityObjectPool.h"

#include "Core/Managers/PoolHandlerSubSystem.h"

APooledObject* AAbilityObjectPool::SpawnPoolObject(FString map)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning object from pool: %s"), *map);

	int index = ++ObjectPool[map].index;
	if(index >= ObjectPool[map].pool.Num())
	{
		index = 0;
		ObjectPool[map].index = 0;
	}
	
	APooledObject* out = ObjectPool[map].pool[index];//= nullptr;

	//if(!out) return nullptr;
	
	//out->SetActiveState(true);
	
	return out;
}

void AAbilityObjectPool::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const world = GetWorld();
	
	verify(world);
	
	const AAbilityObjectPool* inst = GetGameInstance()->GetSubsystem<UPoolHandlerSubSystem>()->GetPool();
	if (inst != nullptr && inst != this)
	{
		Destroy();
		return;
	}
	GetGameInstance()->GetSubsystem<UPoolHandlerSubSystem>()->SetPool(this);
	

	
	for (const auto pool : PooledObjectSubclass)
	{
		verify(pool.pooledSubclass);
		
		FString name = pool.pooledSubclass->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Creating object pool: %s"), *name);
		
		TArray<APooledObject*> stored;
		stored.SetNum(pool.numToSpawn);
		
		for(int i = 0; i <pool.numToSpawn; ++i)
		{
			APooledObject* PooledObject = world->SpawnActor<APooledObject>(pool.pooledSubclass, FVector().ZeroVector, FRotator().ZeroRotator);

			PooledObject->SetActiveState(false);
			//PooledObject->OnPooledObjectDespawn.AddDynamic(this, &AAbilityObjectPool::OnPoolObjectDespawned);
			stored[i] = PooledObject;
		}
		FPoolSet fp;
		fp.pool = stored;
		UE_LOG(LogTemp, Warning, TEXT("Finalized pool with elements: %d"), pool.numToSpawn);
		ObjectPool.Add(name, fp);
	}
}

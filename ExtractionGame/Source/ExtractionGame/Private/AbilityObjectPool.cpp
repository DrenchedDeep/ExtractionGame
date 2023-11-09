// ObjectPool.cpp

#include "AbilityObjectPool.h"

#include "Core/AbilityHandlerSubSystem.h"
#include "Core/PoolHandlerSubSystem.h"

/*Not possible rn
APooledObject* AAbilityObjectPool::EmergencySpawn(const FString& map)
{
	UWorld* const world = GetWorld();
	for (const auto pool : PooledObjectSubclass)
	{
		if(pool.pooledSubclass->GetName() != map) continue;
		APooledObject* PooledObject = world->SpawnActor<APooledObject>(pool.pooledSubclass, FVector().ZeroVector, FRotator().ZeroRotator);
		PooledObject->OnPooledObjectDespawn.AddDynamic(this, &AAbilityObjectPool::OnPoolObjectDespawned);
		return PooledObject;
	}
	return nullptr;
}*/

APooledObject* AAbilityObjectPool::SpawnPoolObject(FString map)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning object from pool: %s"), *map);

	currentIterators[map] = ++currentIterators[map] % ObjectPool[map].Num();
	
	APooledObject* out = ObjectPool[map][currentIterators[map]];//= nullptr;

	
	
	out->SetActivate(true);
		
	return out;
}

void AAbilityObjectPool::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const world = GetWorld();
	GetGameInstance()->GetSubsystem<UPoolHandlerSubSystem>()->SetPool(this);
	
	verify(world);
	
	for (const auto pool : PooledObjectSubclass)
	{
		verify(pool.pooledSubclass);
		UE_LOG(LogTemp, Warning, TEXT("Creating object pool: %s"), *pool.pooledSubclass->GetName());
		FString name = pool.pooledSubclass->GetName();
		ObjectPool.Add(name, TArray<APooledObject*>());
		currentIterators.Add(name, 0);
		ObjectPool[name].SetNum(pool.numToSpawn);
		for(int i = 0; i <pool.numToSpawn; ++i)
		{
			APooledObject* PooledObject = world->SpawnActor<APooledObject>(pool.pooledSubclass, FVector().ZeroVector, FRotator().ZeroRotator);

			PooledObject->SetActivate(false);
			PooledObject->OnPooledObjectDespawn.AddDynamic(this, &AAbilityObjectPool::OnPoolObjectDespawned);
			ObjectPool[name][i] = PooledObject;
		}
	}
}

void AAbilityObjectPool::OnPoolObjectDespawned(APooledObject* obj, FString pool)
{
	UE_LOG(LogTemp, Warning, TEXT("Despawning object to pool: %s"), *pool);
	obj->SetActivate(false);
	//ObjectPool[pool].Enqueue(obj);
	
}

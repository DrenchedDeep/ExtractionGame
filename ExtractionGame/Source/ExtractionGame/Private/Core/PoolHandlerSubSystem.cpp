


#include "Core/PoolHandlerSubSystem.h"

APooledObject* UPoolHandlerSubSystem::SpawnObject(FString type)
{
	if(!Pool)
	{
		UE_LOG(LogTemp, Warning, TEXT("How was the pool null"))
		return nullptr;
	}
	return Pool->SpawnPoolObject(type);
}

void UPoolHandlerSubSystem::SetPool(AAbilityObjectPool* newPool)
{
	Pool = newPool;
}

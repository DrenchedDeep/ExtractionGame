


#include "Core/PoolHandlerSubSystem.h"

APooledObject* UPoolHandlerSubSystem::SpawnObject(FString type)
{

	verify(Pool);
	
	return Pool->SpawnPoolObject(type);
}

void UPoolHandlerSubSystem::SetPool(AAbilityObjectPool* newPool)
{
	Pool = newPool;
}

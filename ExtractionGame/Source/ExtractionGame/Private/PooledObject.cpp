


#include "PooledObject.h"

// Sets default values
APooledObject::APooledObject(): PoolIndex(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APooledObject::Deactivate()
{
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledObjectDespawn.Broadcast(this);
}

void APooledObject::SetActive(bool bIsActive)
{
	bActive = bIsActive;
	SetActorHiddenInGame(!bActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObject::Deactivate, LifeSpan, false);
}

void APooledObject::SetLifeSpan(float LifeTime)
{
	LifeSpan = LifeTime;
}

void APooledObject::SetPoolIndex(int index)
{
	PoolIndex = index;
}

bool APooledObject::IsActive() const
{
	return bActive;
}

int APooledObject::GetPoolIndex() const
{
	return PoolIndex;
}


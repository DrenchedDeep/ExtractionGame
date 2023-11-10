


#include "PooledObject.h"

// Sets default values
APooledObject::APooledObject(): Power(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void APooledObject::SetActiveState(bool isOn)
{
	SetActorHiddenInGame(!isOn);
	SetActorTickEnabled(isOn);
	SetActorEnableCollision(isOn);
	SetActiveCustom(isOn);
}

void APooledObject::ReturnToPool()
{
	//OnPooledObjectDespawn.Broadcast(this, *GetClass()->GetName());
	SetActiveState(false);
}

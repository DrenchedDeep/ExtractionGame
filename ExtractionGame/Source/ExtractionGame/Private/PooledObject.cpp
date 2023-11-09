


#include "PooledObject.h"

// Sets default values
APooledObject::APooledObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void APooledObject::SetActivate(bool isOn)
{
	SetActorHiddenInGame(!isOn);
	SetActorTickEnabled(isOn);
	SetActorEnableCollision(isOn);
}

void APooledObject::Destroyed()
{
	OnPooledObjectDespawn.Broadcast(this, *GetClass()->GetName());
	SetActivate(false);
}

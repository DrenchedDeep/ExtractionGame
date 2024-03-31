


#include "PooledObject.h"

#include "GameFramework/GameNetworkManager.h"
#include "Net/UnrealNetwork.h"

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

void APooledObject::SetLifeSpan(float InLifespan)
{
	//super::SetLifeSpan(InLifespan);
	GetWorldTimerManager().ClearTimer(timer);
	GetWorldTimerManager().SetTimer(timer, this,  &APooledObject::ReturnToPool, InLifespan, false);
}

void APooledObject::ReturnToPool()
{
	GetWorldTimerManager().ClearTimer(timer);
	//OnPooledObjectDespawn.Broadcast(this, *GetClass()->GetName());
	SetActorTickEnabled(false);
	SetActiveState(false);
}

void APooledObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APooledObject, Power)
}

bool APooledObject::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget,
	const FVector& SrcLocation) const
{
	if(!AbilityOwner)
	{
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	}
	
	return (RealViewer && AbilityOwner) && RealViewer->GetUniqueID() != AbilityOwner->GetUniqueID();
}

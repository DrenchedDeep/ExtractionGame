


#include "Core/ExtractionGame/ShipSpawnPoint.h"

// Sets default values
AShipSpawnPoint::AShipSpawnPoint(): innerRing(0), outerRing(0), minHeight(0), maxHeight(0), additionalUpOffset(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AShipSpawnPoint::MoveToPosition(AActor* actor)
{
	if(!actor->HasAuthority()) return;
	const float distance = FMath::RandRange(innerRing, outerRing);
	const float height = FMath::RandRange(minHeight, maxHeight) + additionalUpOffset;
	const float angle = FMath::RandRange(0.f, 6.28f);
	
	const FVector SpawnPoint(FMath::Cos(angle) * distance,FMath::Sin(angle) * distance,height);
	const FRotator Rotation(0,FMath::RadiansToDegrees(angle),0);
	
	actor->SetActorLocationAndRotation(SpawnPoint, Rotation, false);
}


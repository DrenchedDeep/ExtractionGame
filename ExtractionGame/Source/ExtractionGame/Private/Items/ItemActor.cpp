


#include "Items/ItemActor.h"

#include "Kismet/GameplayStatics.h"

AItemActor::AItemActor()
{
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
}

void AItemActor::PostReplication(TArray<uint8>& Payload)
{
}

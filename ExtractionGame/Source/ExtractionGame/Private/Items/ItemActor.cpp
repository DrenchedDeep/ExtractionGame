


#include "Items/ItemActor.h"

#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemSpawner.h"
#include "Net/UnrealNetwork.h"

void AItemActor::UnregisterItem()
{
	if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->Server_PickupItem(this);
	}
	
	Destroy();
}

void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, DefaultPolish);
}

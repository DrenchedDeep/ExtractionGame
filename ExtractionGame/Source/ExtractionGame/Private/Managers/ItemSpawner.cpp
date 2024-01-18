


#include "Managers/ItemSpawner.h"


AItemSpawner::AItemSpawner()
{
#if UE_EDITOR

if(bSpawnOnLandscape)
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), SpawnRadius, 32, FColor::Red, true, 100.f);
}

#endif
}

void AItemSpawner::SpawnItems(UItemReplicationManager* ItemManager)
{
	if(bSpawnOnLandscape)
	{
		SpawnItemsOnLandscape();
	}
	else
	{
		FVector Start = GetActorLocation();
		FVector End = Start - FVector::UpVector * 1000.f;
		
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);


		FVector SpawnLocation;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			SpawnLocation = HitResult.Location;
		}
		else
		{
			SpawnLocation = Start;
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//	UE_LOG(LogTemp, Warning, TEXT("Spawning Item"));
		UE_LOG(LogTemp, Warning, TEXT("Spawn Location: %s"), *SpawnLocation.ToString());
		AItemActor* ItemActor = GetWorld()->SpawnActor<AItemActor>(SpawnableItems[FMath::RandRange(0, SpawnableItems.Num() - 1)],
			SpawnLocation, GetActorRotation(), SpawnParams);

		if(ItemActor)
		{
			ItemActor->Index = ItemManager->RegisterNewItem(ItemActor);
			ItemActor->ItemSpawner = this;
		}
	}

	ItemReplicationManager = ItemManager;
}

void AItemSpawner::SpawnItemsOnLandscape()
{
	
}

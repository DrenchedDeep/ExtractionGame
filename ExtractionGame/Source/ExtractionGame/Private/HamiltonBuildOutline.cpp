


#include "HamiltonBuildOutline.h"

#include "Kismet/GameplayStatics.h"

bool AHamiltonBuildOutline::IsLocationBuildable() const
{
	bool bBuildable = true;

	for(auto GeyserSpawnpoint : GeyserSpawnpoints)
	{
		if(FVector::Dist(GeyserSpawnpoint->GetActorLocation(), GetActorLocation()) < MinDistFromGeyser)
		{
			bBuildable = false;
			break;
		}
	}

	return bBuildable;
}

void AHamiltonBuildOutline::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeyserSpawnpoint::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		AGeyserSpawnpoint* GeyserSpawnpoint = Cast<AGeyserSpawnpoint>(Actor);
		if (GeyserSpawnpoint)
		{
			GeyserSpawnpoints.Add(GeyserSpawnpoint);
		}
	}
}

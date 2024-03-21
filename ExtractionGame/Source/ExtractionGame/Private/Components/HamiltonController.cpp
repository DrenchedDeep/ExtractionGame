


#include "Components/HamiltonController.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void UHamiltonController::StartHamiltonProcess()
{
	if(!IsLocationBuildable())
	{
		return;
	}
	
	Server_HamiltonStarted();
	bIsTickingHamilton = true;
	const float Tickrate = 1.f / 30.f;
	GetWorld()->GetTimerManager().SetTimer(HamiltonBuildTimerHandle, this,
		&UHamiltonController::TickHamiltonBuild, Tickrate, true);

	if(Character)
	{
		Character->HamiltonProcessStarted();
	}
}

void UHamiltonController::StopHamiltonProcess()
{
	if(bIsTickingHamilton)
	{
		GetWorld()->GetTimerManager().ClearTimer(HamiltonBuildTimerHandle);
		bIsTickingHamilton = false;
		HamiltonTickTime = 0.f;
	}

	if(Character)
	{
		Character->HamiltonProcessStopped();
	}
}

void UHamiltonController::OnHamiltonPressed()
{
	StartHamiltonProcess();
}

void UHamiltonController::OnHamiltonReleased()
{
	Server_CancelHamilton();
}

void UHamiltonController::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
}

void UHamiltonController::Server_CancelHamilton_Implementation()
{
	Multicast_UpdateHamilton(true);
}

void UHamiltonController::Multicast_UpdateHamilton_Implementation(bool Cancelled)
{
	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}
	
	if(Character && !Character->IsLocallyControlled())
	{
		Character->HamiltonStartedEverybody(Cancelled);
	}
}


void UHamiltonController::Server_HamiltonStarted_Implementation()
{
	Multicast_UpdateHamilton(false);
}

void UHamiltonController::Server_SpawnHamilton_Implementation(FVector Location)
{
	if(HamiltonActor)
	{
		HamiltonActor->Destroy();
	}

	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}

	if(Character)
	{
		FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * SpawnOffsetFromPlayer.X + Character->GetActorRightVector() * SpawnOffsetFromPlayer.Y + Character->GetActorUpVector() * SpawnOffsetFromPlayer.Z;
		HamiltonActor = GetWorld()->SpawnActor<AActor>(HamiltonActorClass, SpawnLocation, FRotator::ZeroRotator);
	}
}


void UHamiltonController::TickHamiltonBuild()
{
	HamiltonTickTime += 1.f / 30.f;

	if(HamiltonTickTime >=	HamiltonMaxTime)
	{
		StopHamiltonProcess();
	//	Server_SpawnHamilton(LastSpawnLocation);
	}

	if(HamiltonTickTime >= HamiltonSpawnTime)
	{
		Server_SpawnHamilton(LastSpawnLocation);
	}
}

bool UHamiltonController::IsLocationBuildable()
{
	bool bBuildable = true;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeyserSpawnpoint::StaticClass(), FoundActors);
	for(auto GeyserSpawnpoint : FoundActors)
	{
		if(FVector::Dist(GeyserSpawnpoint->GetActorLocation(), Character->GetActorLocation()) < 10000)
		{
			bBuildable = false;
			break;
		}
	}

	//check up
	FHitResult Hit;
	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * SpawnOffsetFromPlayer.X + Character->GetActorRightVector() * SpawnOffsetFromPlayer.Y + Character->GetActorUpVector() * SpawnOffsetFromPlayer.Z;
	FVector End = SpawnLocation + FVector(0, 0, 10000);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Character);
	if(GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation, End, ECC_Visibility, TraceParams))
	{
		bBuildable = false;
	}

	//check down
	End = SpawnLocation - FVector(0, 0, 500);
	if(!GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation, End, ECC_Visibility, TraceParams))
	{
		bBuildable = false;
	}
	
	return bBuildable;
}


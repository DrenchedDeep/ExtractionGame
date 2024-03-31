


#include "Components/HamiltonController.h"

#include "GeyserController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void UHamiltonController::StartHamiltonProcess()
{
	if(Character->GetEssence() < MinEssenceThreshold)
	{
		UE_LOG(LogTemp,Warning,TEXT("Not enough essence, cannot begin process"))
		return;
	}
	
	if(bIsTickingHamilton)
	{
		UE_LOG(LogTemp,Warning,TEXT("Hamilton must already be active, or building"))
		return;
	}
	
	if(!IsLocationBuildable())
	{
		return;
	}
	
	Server_HamiltonStarted();
	bIsTickingHamilton = true;
	constexpr float Tickrate = 1.f / 30.f;
	GetWorld()->GetTimerManager().SetTimer(HamiltonBuildTimerHandle, this,
		&UHamiltonController::TickHamiltonBuild, Tickrate, true);

	if(Character)
	{
		Character->HamiltonProcessStarted();
	}
}

void UHamiltonController::StopHamiltonProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("HamiltonController -> Stop Hamilton Process"))
	if(bIsTickingHamilton)
	{
		GetWorld()->GetTimerManager().ClearTimer(HamiltonBuildTimerHandle);
		bIsTickingHamilton = false;
		HamiltonTickTime = 0.f;
	}

	if(Character)
	{
		Character->HamiltonFinished();
	}
}

void UHamiltonController::OnHamiltonPressed()
{
	StartHamiltonProcess();
}

void UHamiltonController::OnHamiltonReleased()
{
	//Cannot cancel it if any of these succeed.. It doesn't matter if you cancel while one is built or if you can't afford it.
	if(HamiltonBuilt || Character->GetEssence() < MinEssenceThreshold) return;
	
	Server_CancelHamilton();
}

void UHamiltonController::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
}

void UHamiltonController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHamiltonController, HamiltonActor);
}

void UHamiltonController::OnRep_HamiltonActor()
{
	if(Character)
	{
		Character->SetHamiltonActor(HamiltonActor);
	}
}

void UHamiltonController::Client_SetHamilton_Implementation(AActor* InActor)
{
	if(Character)
	{
		Character->SetHamiltonActor(InActor);
	}
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
		HamiltonActor = GetWorld()->SpawnActor<AActor>(HamiltonActorClass, SpawnLocation, FRotator::ZeroRotator );
	}

	//Client_SetHamilton(HamiltonActor);
}


void UHamiltonController::TickHamiltonBuild()
{
	HamiltonTickTime += 1.f / 30.f;

	if(HamiltonTickTime >=	HamiltonMaxTime)
	{
		HamiltonBuilt = false;
		bSentRPC = false;
		StopHamiltonProcess();
	//	Server_SpawnHamilton(LastSpawnLocation);
	}

	if(HamiltonTickTime >= HamiltonSpawnTime && !bSentRPC)
	{
		HamiltonBuilt = true;
		Server_SpawnHamilton(LastSpawnLocation);
		bSentRPC = true;
	}
}

bool UHamiltonController::IsLocationBuildable()
{
	bool bBuildable = true;

	TArray<AGeyserSpawnpoint*> FoundActors = AGeyserController::GetGeyserSpawns();
	for(auto GeyserSpawnpoint : FoundActors)
	{
		//Why do we care if the geyser isn't active?
		if(GeyserSpawnpoint->GetGeyserState() != EGeyserStates::Active) continue;
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
	End = SpawnLocation - FVector(0, 0, 50);
	if(!GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation, End, ECC_Visibility, TraceParams))
	{
		bBuildable = false;
	}
	
	return bBuildable;
}





#include "Components/HamiltonController.h"

#include "Camera/CameraComponent.h"


void UHamiltonController::StartHamiltonProcess()
{
	if(BuildOutlineActor)
	{
		if(!BuildOutlineActor->IsLocationBuildable())
		{
			return;
		}
	}

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

	if(BuildOutlineActor)
	{
		BuildOutlineActor->Destroy();
	}

	if(Character)
	{
		Character->HamiltonProcessStopped();
	}
}

void UHamiltonController::OnHamiltonPressed()
{
	if(BuildOutlineActor)
	{
		BuildOutlineActor->Destroy();
	}

	 BuildOutlineActor = GetWorld()->SpawnActor<AHamiltonBuildOutline>(BuildOutlineActorClass, FVector::ZeroVector, FRotator::ZeroRotator);

	float Tickrate = 1.f / 30.f;
	GetWorld()->GetTimerManager().SetTimer(BuildOutlineTimerHandle, this,
		&UHamiltonController::TickBuildOutline, Tickrate, true);

	bIsBuildingOutline = true;
}

void UHamiltonController::OnHamiltonReleased()
{
	if(BuildOutlineActor)
	{
		BuildOutlineActor->Destroy();
	}

	GetWorld()->GetTimerManager().ClearTimer(BuildOutlineTimerHandle);
	bIsBuildingOutline = false;
}

void UHamiltonController::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
}

void UHamiltonController::Server_SpawnHamilton_Implementation(FVector Location)
{
	if(HamiltonActor)
	{
		HamiltonActor->Destroy();
	}

	HamiltonActor = GetWorld()->SpawnActor<AActor>(HamiltonActorClass, Location, FRotator::ZeroRotator);
}

void UHamiltonController::TickBuildOutline()
{
	if(BuildOutlineActor)
	{
		BuildOutlineActor->UpdateMaterials();
		FHitResult Hit;

		const FVector StartLocation = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		 FVector EndLocation = StartLocation + Character->GetFirstPersonCameraComponent()->GetForwardVector() * 1000.f;
		if(GetWorld()->LineTraceSingleByChannel(Hit, StartLocation,EndLocation, ECC_Visibility))
		{
			BuildOutlineActor->SetActorLocation(Hit.Location);
			LastSpawnLocation = Hit.Location;
		}
		else
		{
			EndLocation = EndLocation + FVector::DownVector * 1000.f;

			if(GetWorld()->LineTraceSingleByChannel(Hit, StartLocation,EndLocation, ECC_Visibility))
			{
				BuildOutlineActor->SetActorLocation(Hit.Location);
				LastSpawnLocation = Hit.Location;
			}
		}
	}
}

void UHamiltonController::TickHamiltonBuild()
{
	HamiltonTickTime += 1.f / 30.f;

	if(HamiltonTickTime >=	HamiltonMaxTime)
	{
		StopHamiltonProcess();
		Server_SpawnHamilton(LastSpawnLocation);
		
	}
}

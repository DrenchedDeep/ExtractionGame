


#include "Extraction/ExtractionBeacon.h"

#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UI/ExtractionGameHUD.h"

AExtractionBeacon::AExtractionBeacon()
{
	bReplicates = true;
}

void AExtractionBeacon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionBeacon, bIsExtracting);
	DOREPLIFETIME(AExtractionBeacon, ExtractionTimer);
}

void AExtractionBeacon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(HasAuthority() && bIsExtracting && !bExtractionCompleted)
	{
		ExtractionTimer -= DeltaSeconds;
		OnRep_ExtractionTimer();


		if(ExtractionTimer <= 0)
		{
			OnExtractionCompleted();
		}
	}
}

void AExtractionBeacon::PlayerEnter(AExtractionGameCharacter* Character)
{
	if(!HasAuthority())
	{
		return;
	}
	
	if(!Character)
	{
		return;
	}

	if(!Character->GetController())
	{
		return;
	}

	if(Cast<APlayerController>(Character->GetController()) == nullptr)
	{
		return;
	}

	if(PlayerCanEnterExtractionBeacon(Cast<APlayerController>(Character->GetController())))
	{
		Players.Add(Character);

		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Character->GetController()))
		{
			PC->Client_EnteredExtractionBeacon(this);
		}
	}
}

void AExtractionBeacon::PlayerExit(AExtractionGameCharacter* Character)
{
	if(Character && HasAuthority())
	{
		Players.Remove(Character);

		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Character->GetController()))
		{
			PC->Client_LeftExtractionBeacon();
		}
	}
}



void AExtractionBeacon::StartExtraction()
{
	bIsExtracting = true;
	OnRep_IsExtracting();
	UE_LOG(LogTemp, Warning, TEXT("Extraction Started"));
	ExtractionTimer = ExtractionMaxTimer;

	for(auto Player : Players)
	{
		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Player->GetController()))
		{
			PC->Client_EnteredExtractionBeacon(this);
		}
	}
}


void AExtractionBeacon::OnRep_ExtractionTimer()
{
	APlayerController* LocalPlayer = GetWorld()->GetFirstPlayerController();

	if(LocalPlayer)
	{
		if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(LocalPlayer->GetHUD()))
		{
			HUD->UpdateExtractionTimerWidget(ExtractionTimer);
		}
	}
}

void AExtractionBeacon::OnRep_IsExtracting()
{
	APlayerController* LocalPlayer = GetWorld()->GetFirstPlayerController();

	if(LocalPlayer)
	{
		for(auto Player : Players)
		{
			if(AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(LocalPlayer->GetHUD()))
			{
				HUD->ExtractionWidget->AddToViewport();
			}
		}
	}
}

void AExtractionBeacon::OnExtractionCompleted()
{
	bExtractionCompleted = true;

	for(const auto Player : Players)
	{
		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Player->GetController()))
		{
			PC->ReturnToLobby();
		}
	}
}

bool AExtractionBeacon::PlayerCanEnterExtractionBeacon(APlayerController* PC)
{
	return true;
}




#include "AI/Goober.h"

#include "ExtractionGamePlayerState.h"
#include "GooberSpawnCluster.h"
#include "Net/UnrealNetwork.h"

void AGoober::ApplyDamage(float Damage, AController* Killer)
{
	if(CurrentHealth == 0 ) return;  
	CurrentHealth -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);
	if(CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		bIsDead = true;
		BP_OnGooberDeadServer();
		if(GooberSpawn)
		{
			GooberSpawn->UnRegisterGoober(this);
		}

		if (Killer == nullptr) return;

		if(AExtractionGamePlayerState* InstigatorPlayerState = Killer->GetPlayerState<AExtractionGamePlayerState>())
		{
			InstigatorPlayerState->AddGooberKill();
			UE_LOG(LogTemp,Warning, TEXT("GOOBER KILLED"))
		}
	}
}

void AGoober::MoveToGeyser(AActor* Geyser)
{
	BP_MoveToGeyser(Geyser);
}

void AGoober::StartCook()
{
	bIsCooking = true;
	OnRep_Cooking(); // Do you need to manually do this? I thought replicatingUsing did this for you?
	UE_LOG(LogTemp, Warning, TEXT("StartCook"));
}

void AGoober::StopCook()
{
	bIsCooking = false;
	OnRep_Cooking();
	UE_LOG(LogTemp, Warning, TEXT("StopCook"));
}

void AGoober::Init(AGooberSpawnCluster* InGooberSpawnCluster)
{
	GooberSpawn = InGooberSpawnCluster;
}

void AGoober::OnRep_Cooking()
{
	if(bIsCooking && !bIsCooked && bIsDead) // Do not cook twice... And the guy needa be dead.
	{
		//OnStartCook();
		//CurrentCookTimer = 0; // Clearing the timer like this may be unn
		GetWorldTimerManager().SetTimer(CookTimerHandle, this, &AGoober::OnCookTimerTick, 1.f, true);
	}
	else
	{
		//OnCookTimerEnd();
		//OnEndCook();
		if(CookTimerHandle.IsValid()) GetWorldTimerManager().PauseTimer(CookTimerHandle);
	}
}

void AGoober::OnCookTimerTick()
{
	CurrentCookTimer += 1;
	if(CurrentCookTimer >= MaxCookTime)
	{
		bIsCooked = true;
		GetWorldTimerManager().ClearTimer(CookTimerHandle);
		CookTimerHandle.Invalidate();
		CurrentCookTimer = MaxCookTime;
	}
	Execute_OnCookTick(this, CurrentCookTimer / MaxCookTime);
}


void AGoober::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGoober, bIsDead);
	DOREPLIFETIME(AGoober, CurrentCookTimer);
	DOREPLIFETIME(AGoober, bIsCooked);
	DOREPLIFETIME(AGoober, GooberState);

}

void AGoober::OnRep_Dead()
{
	BP_OnGooberDeadAll();
}

void AGoober::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		CurrentHealth = MaxHealth;
	}
}

void AGoober::OnRep_BoogerAids()
{
	if(CurrentCookTimer >= MaxCookTime)
	{
		bIsCooked = true;
		GetWorldTimerManager().ClearTimer(CookTimerHandle);
		CookTimerHandle.Invalidate();
		CurrentCookTimer = MaxCookTime;
	}
	Execute_OnCookTick(this, CurrentCookTimer / MaxCookTime);
}

void AGoober::AddCook(float amount)
{
	CurrentCookTimer += amount;
	if(CurrentCookTimer >= MaxCookTime)
	{
		bIsCooked = true;
		GetWorldTimerManager().ClearTimer(CookTimerHandle);
		CookTimerHandle.Invalidate();
		CurrentCookTimer = MaxCookTime;
	}
	Execute_OnCookTick(this, CurrentCookTimer / MaxCookTime);
}

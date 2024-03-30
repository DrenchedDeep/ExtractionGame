


#include "AI/Goober.h"

#include "GooberSpawnCluster.h"
#include "Net/UnrealNetwork.h"

void AGoober::ApplyDamage(float Damage)
{
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

	Execute_OnCookTick(this, CurrentCookTimer / MaxCookTime);
	//UpdateCookedProgress();
	
	if(CurrentCookTimer >= MaxCookTime)
	{
		bIsCooked = true;
		GetWorldTimerManager().ClearTimer(CookTimerHandle);
		CookTimerHandle.Invalidate();
	}
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




#include "AI/Goober.h"

#include "Net/UnrealNetwork.h"

void AGoober::ApplyDamage(float Damage)
{
	CurrentHealth -= Damage;

	if(CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		bIsDead = true;
		BP_OnGooberDeadServer();
	}
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

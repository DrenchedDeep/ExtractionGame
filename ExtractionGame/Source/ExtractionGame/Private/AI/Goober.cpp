


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
	OnRep_Cooking();
	UE_LOG(LogTemp, Warning, TEXT("StartCook"));
}

void AGoober::OnRep_Cooking()
{
	if(bIsCooking)
	{
		OnCookTimerStart();
		CurrentCookTimer = MaxCookTime;
		GetWorldTimerManager().SetTimer(CookTimerHandle, this, &AGoober::OnCookTimerTick, 1.f, true);
	}
	else
	{
		OnCookTimerEnd();

		GetWorldTimerManager().ClearTimer(CookTimerHandle);
	}
}

void AGoober::OnCookTimerTick()
{
	CurrentCookTimer -= 1.f;

	if(CurrentCookTimer <= 0)
	{
		bIsCooked = true;
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




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

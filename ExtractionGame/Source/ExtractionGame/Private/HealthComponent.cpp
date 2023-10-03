


#include "Components/HealthComponent.h"

//#include "../../../../../../../UE_5.3_Source/UnrealEngine-release/Engine/Source/Runtime/Engine/Public/Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{

}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::ApplyDamage(FDamageData DamageInfo, bool bClientSimulation)
{
	if(bClientSimulation)
	{
		OnRep_CurrentHealth();
		return;
	}
	
	CurrentHealth -= DamageInfo.Damage;

	if(CurrentHealth <= 0.f)
	{
		OnDeath();
	}
}


void UHealthComponent::OnDeath()
{
	CurrentHealth = 0.f;
}

void UHealthComponent::OnRep_CurrentHealth()
{
	
}

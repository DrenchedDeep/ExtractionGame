#include "PlayerHealthComponent.h"

#include "Net/UnrealNetwork.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
	//SetIsReplicated(true);
}

void UPlayerHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHealthComponent, CurrentHealth);
}

void UPlayerHealthComponent::ApplyDamage(FDamageInfo DamageInfo)
{
	CurrentHealth -= DamageInfo.Damage;
	
	if(CurrentHealth <= 0)
	{
		
	}
}

void UPlayerHealthComponent::Client_DamageReceived_Implementation()
{
	
}

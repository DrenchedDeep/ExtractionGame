#include "PlayerHealthComponent.h"

#include "ExtractionGameCharacter.h"


UPlayerHealthComponent::UPlayerHealthComponent(): Character(nullptr)
{
}

void UPlayerHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
}

void UPlayerHealthComponent::ApplyDamage(FDamageData DamageInfo, bool bClientSimulation)
{
	if(CurrentHealth > 0.f)
	{
		Client_ApplyDamage();
	}

	Super::ApplyDamage(DamageInfo, bClientSimulation);
}

void UPlayerHealthComponent::OnDeath()
{
	
}

void UPlayerHealthComponent::Client_ApplyDamage_Implementation()
{
}

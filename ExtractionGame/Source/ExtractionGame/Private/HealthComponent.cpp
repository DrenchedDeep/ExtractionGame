


#include "Components/HealthComponent.h"

#include "ExtractionGame/ExtractionGameCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

//#include "../../../../../../../UE_5.3_Source/UnrealEngine-release/Engine/Source/Runtime/Engine/Public/Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{

}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, bIsDead);
}

void UHealthComponent::ApplyDamage(float Damage, const AController* Instigator)
{
	if(!bCanTakeDamage)
	{
		return;
	}
	
	CurrentHealth -= Damage;
	OnRep_CurrentHealth();

	if(CurrentHealth <= 0)
	{
		bCanTakeDamage = false;
		AExtractionGameCharacter* Character = Cast<AExtractionGameCharacter>(GetOwner());
		OnDeath(Character->GetPlayerState()->GetPlayerName());
	}
}

void UHealthComponent::OnDeath(const FString& PlayerName)
{
	CurrentHealth = 0;
	bIsDead = true;
	OnRep_IsDead();
}


void UHealthComponent::OnRep_CurrentHealth()
{
	
}

void UHealthComponent::OnRep_IsDead()
{
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bCanTakeDamage = true;
}

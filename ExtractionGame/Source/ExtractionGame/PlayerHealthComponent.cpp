#include "PlayerHealthComponent.h"
#include "ExtractionGameCharacter.h"
#include "ExtractionGameHUD.h"
#include "ExtractionGamePlayerController.h"
#include "TDMPlayerState.h"
#include "Net/UnrealNetwork.h"

UPlayerHealthComponent::UPlayerHealthComponent(): Character(nullptr)
{
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
	bIsDead = false;

}

void UPlayerHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHealthComponent, bIsDead);
	DOREPLIFETIME(UPlayerHealthComponent, HitCount);
}


void UPlayerHealthComponent::OnDeath(const FString& PlayerName)
{
	bIsDead = true;
	OnRep_IsDead();

	AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Character->GetController());

	PC->OnDeath(PlayerName);
}

void UPlayerHealthComponent::OnRep_IsDead()
{
	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}
	
	Character->OnDeathEvent();
}

void UPlayerHealthComponent::OnRep_HitCounter()
{
}

void UPlayerHealthComponent::ApplyDamage(float Damage, const AController* Instigator)
{
	if(!bCanTakeDamage)
	{
		return;
	}
	
	if(ATDMPlayerState* OwnerPlayerState = Character->GetPlayerState<ATDMPlayerState>())
	{
		if(ATDMPlayerState* InstigatorPlayerState = Instigator->GetPlayerState<ATDMPlayerState>())
		{
			if(OwnerPlayerState->TeamID == InstigatorPlayerState->TeamID)
			{
				return;
			}
		}
	}
	
	float Health = GetCurrentHealth() - Damage;
	Character->GetPlayerState<AGemPlayerState>()->SetHealth(Health);
	
	HitCount++;
	OnRep_HitCounter();

	if(Health <= 0)
	{
		bCanTakeDamage = false;
		OnDeath(Instigator->PlayerState->GetPlayerName());
	}
}

void UPlayerHealthComponent::Client_ApplyDamage_Implementation()
{
}

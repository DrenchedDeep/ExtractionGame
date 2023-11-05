#include "PlayerHealthComponent.h"

#include "ExtractionGameCharacter.h"
#include "ExtractionGameHUD.h"
#include "ExtractionGamePlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpectatorPawn.h"


UPlayerHealthComponent::UPlayerHealthComponent(): Character(nullptr)
{
}

void UPlayerHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Character = Cast<AExtractionGameCharacter>(GetOwner());
}


void UPlayerHealthComponent::OnDeath(const FName& PlayerName)
{
	Super::OnDeath(PlayerName);

	AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Character->GetController());

	PC->OnDeath(PlayerName);
}

void UPlayerHealthComponent::OnRep_IsDead()
{
	Super::OnRep_IsDead();

	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}
	

	Character->OnDeathEvent();
}

void UPlayerHealthComponent::ApplyDamage(float Damage, const AController* Instigator)
{
	Super::ApplyDamage(Damage, Instigator);

	Client_ApplyDamage();
}

void UPlayerHealthComponent::Client_ApplyDamage_Implementation()
{
}

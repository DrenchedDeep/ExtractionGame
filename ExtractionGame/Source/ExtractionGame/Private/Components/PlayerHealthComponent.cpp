#include "Components/PlayerHealthComponent.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "UI/ExtractionGameHUD.h"
#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Core/ExtractionGame/ExtractionGameState.h"
#include "Core/ExtractionGame/TDMPlayerState.h"
#include "Net/UnrealNetwork.h"

float UPlayerHealthComponent::GetHealth() const
{
	return Character->GetAttributeSet()->GetHealth();
}

float UPlayerHealthComponent::GetMaxHealth() const
{
	return Character->GetAttributeSet()->GetMaxHealth();
}

void UPlayerHealthComponent::SetHealth(float Health, const AController* Instigator)
{
	Character->GetAttributeSet()->SetHealth(Health);
	if(Health < 0)
	{
		bCanTakeDamage = false;

		if(AExtractionGamePlayerState* OwnerPlayerState = Character->GetPlayerState<AExtractionGamePlayerState>())
		{
			OwnerPlayerState->AddDeath();
		}
		
		if(!Instigator)
		{
			OnDeath("WORLD");
			return;
		}

		if(AExtractionGamePlayerState* InstigatorPlayerState = Instigator->GetPlayerState<AExtractionGamePlayerState>())
		{
			InstigatorPlayerState->AddKill();

			OnDeath(Instigator->PlayerState->GetPlayerName());
		}
	}
}

void UPlayerHealthComponent::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarDataWidget* hud =GetHUDElement();
	const float perc = Data.NewValue / GetMaxHealth();
	Character->OnHealthChanged(perc);
	if(!hud) return;
	hud->SetHealthPercent(perc);
}

void UPlayerHealthComponent::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{

	UPlayerBarDataWidget* hud =GetHUDElement();
	if(!hud) return;
	hud->SetHealthPercent(GetHealth() / Data.NewValue);
}

UPlayerBarDataWidget* UPlayerHealthComponent::GetHUDElement()
{
	if(PlayerBarsWidget) return PlayerBarsWidget;
	if(const AExtractionGamePlayerController* x = Cast<AExtractionGamePlayerController>(Character->GetController()))
	{
		if(const AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(x->GetHUD()))
		{
			PlayerBarsWidget = HUD->GetPlayerBarWidget();
		}
	}
	return nullptr;
}

UPlayerHealthComponent::UPlayerHealthComponent(): Character(nullptr), PlayerBarsWidget(nullptr), bIsDead(false)
{
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AExtractionGameCharacter>(GetOwner());
	//if(const AExtractionGameHUD* hud = Cast<AExtractionGameHUD>(Character->GetController<AExtractionGamePlayerController>()->GetHUD()))
	//	GameHUD = hud->PlayerUIData;
	bIsDead = false;

	if(Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		if(const AExtractionGamePlayerController* x = Cast<AExtractionGamePlayerController>(Character->GetController()))
		{
			if(const AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(x->GetHUD()))
			{
				if(HUD->GetPlayerBarWidget())
				{
					HUD->GetPlayerBarWidget()->SetHealthPercent(100.f);
				}
			}
		}
	
		OnHealthChangedHandle =
			Character->GetAbilitySystemComponent()->
		GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &UPlayerHealthComponent::OnHealthChanged);
		OnMaxHealthChangedHandle =
			Character->GetAbilitySystemComponent()->
		GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxHealthAttribute()).AddUObject(this, &UPlayerHealthComponent::OnMaxHealthChanged);
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded Player Health Component"))
	ApplyEffect(&HealthPoolHandle, HealthPoolEffect, 1);
	ApplyEffect(&HealthRegenHandle, HealthRegenEffect, 1);
	Character->OnHealthChanged(1);
}

void UPlayerHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHealthComponent, bIsDead);
}


void UPlayerHealthComponent::OnDeath(const FString& PlayerName)
{
	bIsDead = true;
	OnRep_IsDead();

	if(Character)
	{
		Character->OnDeathServer();
	}
	
	
	AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Character->GetController());

	PC->OnDeath(PlayerName);
}

void UPlayerHealthComponent::Client_ApplyDamage_Implementation(FVector FromDirection)
{
	if(Character && Character->IsLocallyControlled())
	{
		Character->OnAttacked(FromDirection);
	}
}

void UPlayerHealthComponent::OnRep_IsDead()
{
	if(!Character)
	{
		Character = Cast<AExtractionGameCharacter>(GetOwner());
	}

	if(Character)
	{
		if(Character->IsLocallyControlled())
		{
			Character->OnDeathClient();
		}
		Character->OnDeathEvent();
	}
}


void UPlayerHealthComponent::Initialize(const AExtractionGameHUD* hud)
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing Player Health Component"));
	if (UPlayerBarDataWidget* PlayerBarWidget = hud->GetPlayerBarWidget())
	{
		// Use PlayerBarWidget since it's not null
		PlayerBarsWidget = PlayerBarWidget;
		UE_LOG(	LogTemp, Warning, TEXT("PlayerBarWidget is not null"));
		PlayerBarsWidget->SetHealthPercent(100.f);
	}
}

bool UPlayerHealthComponent::ApplyDamage(float Damage, AController* Instigator)
{
	if(!bCanTakeDamage)
	{
		return false;
	}
/*/
	//If the user shares a team.
	if(ATDMPlayerState* OwnerPlayerState = Character->GetPlayerState<ATDMPlayerState>())
	{
		if(ATDMPlayerState* InstigatorPlayerState = Instigator->GetPlayerState<ATDMPlayerState>())
		{
			if(OwnerPlayerState->TeamID == InstigatorPlayerState->TeamID)
			{
			//	return false;
			}
		}
	}


	/*/

	/*/
	if(Character && Instigator)
	{
		if(AExtractionGamePlayerController* PC = Cast<AExtractionGamePlayerController>(Instigator))
		{
			TArray<APawn*> PartyPlayers = PC->GetPartyMemberPawns();
			for(const APawn* PartyPlayer : PartyPlayers)
			{
				if(PartyPlayer == Character)
				{
					//return false;
				}
			}
		}
	}
	/*/


	if(Instigator && Instigator->GetPawn())
	{
		Client_ApplyDamage(Instigator->GetPawn()->GetActorLocation());
	}

	if(AExtractionGameState* GS = GetWorld()->GetGameState<AExtractionGameState>())
	{
		GS->UpdateTotalPlayerKills();
	}
	
	SetHealth(GetHealth() - Damage, Instigator);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), GetHealth());
	return true;
}
void UPlayerHealthComponent::ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const
{
	UE_LOG(LogTemp, Warning, TEXT("APPLY EFFECT CHECK A"))
	
	//If we were previously generating, STOP.
	if(handle->IsValid()) Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(*handle);
	
	FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(Character);

	//TODO: Change level
	UE_LOG(LogTemp, Warning, TEXT("APPLY EFFECT CHECK B"))
	const FGameplayEffectSpecHandle NewHandle = Character->GetAbilitySystemComponent()->MakeOutgoingSpec(effect, level,EffectContext);
	if (NewHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("APPLY EFFECT CHECK C"))
		*handle = Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),  Character->GetAbilitySystemComponent());
	}
}
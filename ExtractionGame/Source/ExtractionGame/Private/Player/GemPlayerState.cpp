


#include "Player/GemPlayerState.h"

#include "ExtractionGameHUD.h"
#include "Abilities/ExtractionAttributeSet.h"
#include "Components/ExtractionAbilitySystemComponent.h"

AGemPlayerState::AGemPlayerState()
{
	//Force replication and abilities on player class.
	AbilitySystemComponent = CreateDefaultSubobject<UExtractionAbilitySystemComponent>(TEXT("GAS Ability Controller"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	AttributeSetBase = CreateDefaultSubobject<UExtractionAttributeSet>(TEXT("GAS Attribute Set"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 20.0f;

	//DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UPlayerBarData* AGemPlayerState::GetUIElement() const
{
	return GameHUD;
}


void AGemPlayerState::BeginPlay()
{
	Super::BeginPlay();
	 if(AExtractionGameHUD* hud = Cast<AExtractionGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	 {
	 	GameHUD = hud->CreatePlayerBarDataWidget();
	 }
	else return;
	

	
	if(!AbilitySystemComponent) return;
	
	//Inject Delegate functions
	OnHealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGemPlayerState::OnHealthChanged);
	OnMaxHealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGemPlayerState::OnMaxHealthChanged);
	
	OnEarthManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetEarthManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnEarthManaChanged);
	OnMaxEarthManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxEarthManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnMaxEarthManaChanged);
	OnFireManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetFireManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnFireManaChanged);
	OnMaxFireManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxFireManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnMaxFireManaChanged);
	OnShadowManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetShadowManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnShadowManaChanged);
	OnMaxShadowManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxShadowManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnMaxShadowManaChanged);
	OnWaterManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetWaterManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnWaterManaChanged);
	OnMaxWaterManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxWaterManaPoolAttribute()).AddUObject(this, &AGemPlayerState::OnMaxWaterManaChanged);
	

}

void AGemPlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetHealthPercent(Data.NewValue / GetMaxHealth());
}

void AGemPlayerState::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetHealthPercent(GetHealth() / Data.NewValue);
}

void AGemPlayerState::OnEarthManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetEarthManaPercent(Data.NewValue / GetEarthMaxMana());
}

void AGemPlayerState::OnMaxEarthManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetEarthManaPercent(GetEarthMana() / Data.NewValue);
}

void AGemPlayerState::OnFireManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetFireManaPercent(Data.NewValue / GetFireMaxMana());
}

void AGemPlayerState::OnMaxFireManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetFireManaPercent(GetFireMana() / Data.NewValue);
}

void AGemPlayerState::OnShadowManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetShadowManaPercent(Data.NewValue / GetShadowMaxMana());
}

void AGemPlayerState::OnMaxShadowManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetShadowManaPercent(GetShadowMana() / Data.NewValue);
}

void AGemPlayerState::OnWaterManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetWaterManaPercent(Data.NewValue / GetWaterMaxMana());
}

void AGemPlayerState::OnMaxWaterManaChanged(const FOnAttributeChangeData& Data)
{
	GetUIElement()->SetWaterManaPercent(GetWaterMana() / Data.NewValue);
}

UAbilitySystemComponent* AGemPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UExtractionAttributeSet* AGemPlayerState::GetAttributeSet() const
{
	return AttributeSetBase;
}

float AGemPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AGemPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AGemPlayerState::GetEarthMana() const
{
	return AttributeSetBase->GetEarthManaPool();
}

float AGemPlayerState::GetEarthMaxMana() const
{
	return AttributeSetBase->GetMaxEarthManaPool();
}

float AGemPlayerState::GetFireMana() const
{
	return AttributeSetBase->GetFireManaPool();
}

float AGemPlayerState::GetFireMaxMana() const
{
	return AttributeSetBase->GetMaxFireManaPool();
}

float AGemPlayerState::GetShadowMana() const
{
	return AttributeSetBase->GetShadowManaPool();
}

float AGemPlayerState::GetShadowMaxMana() const
{
	return AttributeSetBase->GetMaxShadowManaPool();
}

float AGemPlayerState::GetWaterMana() const
{
	return AttributeSetBase->GetWaterManaPool();
}

float AGemPlayerState::GetWaterMaxMana() const
{
	return AttributeSetBase->GetMaxWaterManaPool();
}

float AGemPlayerState::GetManaRegenRate() const
{
	return AttributeSetBase->GetRegenMana();
}

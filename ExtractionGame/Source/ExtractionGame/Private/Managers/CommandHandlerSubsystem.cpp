


#include "Managers/CommandHandlerSubsystem.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Core/Managers/AbilityHandlerSubSystem.h"
#include "Net/UnrealNetwork.h"

void UCommandHandlerSubsystem::SetTimeOfDay(float time)
{
}

void UCommandHandlerSubsystem::GiveItem(FName name)
{
}

void UCommandHandlerSubsystem::TPAllToMe()
{
}

void UCommandHandlerSubsystem::TPAllToLocation(float x, float y, float z)
{
}

void UCommandHandlerSubsystem::EnableDebugInfo()
{
}

void UCommandHandlerSubsystem::KillSelf()
{
}

void UCommandHandlerSubsystem::SetAllManaRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetEarthRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetFireRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetWaterRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetShadowRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::ChangeKnockBackMultiplier(float amt)
{
}

void UCommandHandlerSubsystem::SetDamageMultiplier(float val)
{
}

void UCommandHandlerSubsystem::SetFireRateMultiplier(float val)
{
}

void UCommandHandlerSubsystem::SetCostMultiplier(float val)
{
}

void UCommandHandlerSubsystem::SetAbilityDamage(int E, int F, int S, int W, float val)
{
}

void UCommandHandlerSubsystem::SetAbilityFireRate(int E, int F, int S, int W, float val)
{
}

void UCommandHandlerSubsystem::SetAbilityCost(int E, int F, int S, int W, float val) const
{
	const int32 index = AbilitiesSubSystem->ConvertToIntID(E,F,S,W);

	
}

void UCommandHandlerSubsystem::SetAbilityCostServer_Implementation(int idx, float val)
{
	//Modify the ability cost...
	const UGameplayAbility* ability = Cast<UGameplayAbility>(AbilitiesSubSystem->GetAbilityByIndex(idx));

	FGameplayModifierInfo mod = ability->GetCostGameplayEffect()->Modifiers[0];

	mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-val));
}

bool UCommandHandlerSubsystem::SetAbilityCostServer_Validate(int idx, float val)
{
	return GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() && AbilitiesSubSystem && AbilitiesSubSystem->ContainsAbility(idx);
}

void UCommandHandlerSubsystem::SetAbilityFireRateServer_Implementation(int idx, float val)
{
	//Modify the ability cost...
	const UGameplayAbility* ability = Cast<UGameplayAbility>(AbilitiesSubSystem->GetAbilityByIndex(idx));

	FGameplayModifierInfo mod = ability->GetCostGameplayEffect()->Modifiers[0];

	mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-val));
}

bool UCommandHandlerSubsystem::SetAbilityFireRateServer_Validate(int idx, float val)
{
	return GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() && AbilitiesSubSystem && AbilitiesSubSystem->ContainsAbility(idx);
}

void UCommandHandlerSubsystem::SetAbilityDamageServer_Implementation(int idx, float val)
{
	//Modify the ability cost...
	const UGameplayAbility* ability = Cast<UGameplayAbility>(AbilitiesSubSystem->GetAbilityByIndex(idx));

	FGameplayModifierInfo mod = ability->GetCostGameplayEffect()->Modifiers[0];

	mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-val));
}

bool UCommandHandlerSubsystem::SetAbilityDamageServer_Validate(int idx, float val)
{
	return GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() && AbilitiesSubSystem && AbilitiesSubSystem->ContainsAbility(idx);
}


void UCommandHandlerSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCommandHandlerSubsystem, KnockBackMultiplier)
	DOREPLIFETIME(UCommandHandlerSubsystem, DamageMultiplier)
	DOREPLIFETIME(UCommandHandlerSubsystem, FireRate)
}

void UCommandHandlerSubsystem::Init(UAbilityHandlerSubSystem* abilitySys)
{
	AbilitiesSubSystem = abilitySys;
}


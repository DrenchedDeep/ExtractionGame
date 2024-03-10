


#include "Core/ExtractionGame/CustomGameplayAbility.h"

void UCustomGameplayAbility::SetCurrentActivationInfo(const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::SetCurrentActivationInfo(ActivationInfo);
	OverridenSetActorInfo();
}

UGameplayEffect* UCustomGameplayAbility::GetCostGameplayEffect() const
{
	return OverridenCostEffect.GetDefaultObject();
}

UGameplayEffect* UCustomGameplayAbility::GetCooldownGameplayEffect() const
{
	return OverridenCooldownEffect.GetDefaultObject();
}

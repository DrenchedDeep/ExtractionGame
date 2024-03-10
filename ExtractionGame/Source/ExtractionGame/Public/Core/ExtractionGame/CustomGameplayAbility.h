

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CustomGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UCustomGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


	virtual void SetCurrentActivationInfo(const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual UGameplayEffect* GetCostGameplayEffect() const override;
	virtual UGameplayEffect* GetCooldownGameplayEffect() const override;



	

protected:
	UFUNCTION(BlueprintImplementableEvent) void OverridenSetActorInfo();
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) TSubclassOf<UGameplayEffect> OverridenCostEffect;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) TSubclassOf<UGameplayEffect> OverridenCooldownEffect;
};

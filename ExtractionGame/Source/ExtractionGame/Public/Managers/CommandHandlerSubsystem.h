

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommandHandlerSubsystem.generated.h"

class UAbilityHandlerSubSystem;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UCommandHandlerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//WORLD	
	UFUNCTION(Exec) void SetTimeOfDay(float time);
	UFUNCTION(Exec) void GiveItem(FName name);
	UFUNCTION(Exec) void TPAllToMe();
	UFUNCTION(Exec) void TPAllToLocation(float x, float y, float z);
	UFUNCTION(Exec) void EnableDebugInfo();
	UFUNCTION(Exec) void KillSelf();

	//MANA
	UFUNCTION(Exec) void SetAllManaRegenSpeed(float val);
	UFUNCTION(Exec) void SetEarthRegenSpeed(float val);
	UFUNCTION(Exec) void SetFireRegenSpeed(float val);
	UFUNCTION(Exec) void SetWaterRegenSpeed(float val);
	UFUNCTION(Exec) void SetShadowRegenSpeed(float val);

	//COMBAT
	UFUNCTION(Exec) void ChangeKnockBackMultiplier(float amt);
	
	
	//GENERAL
	UFUNCTION(Exec) void SetDamageMultiplier(float val);
	UFUNCTION(Exec) void SetFireRateMultiplier(float val);
	UFUNCTION(Exec) void SetCostMultiplier(float val);
	
	//SPECIFIC
	UFUNCTION(Exec) void SetAbilityDamage(int E, int F, int S, int W, float val);
	UFUNCTION(Exec) void SetAbilityFireRate(int E, int F, int S, int W, float val);
	UFUNCTION(Exec) void SetAbilityCost(int E, int F, int S, int W, float val) const;

	UFUNCTION(Server, Reliable, WithValidation) void SetAbilityDamageServer(int idx, float val);
	UFUNCTION(Server, Reliable, WithValidation) void SetAbilityFireRateServer(int idx, float val);
	UFUNCTION(Server, Reliable, WithValidation) void SetAbilityCostServer(int idx, float val);
	
	
	
protected:
	UPROPERTY(Replicated) float KnockBackMultiplier = 1;
	UPROPERTY(Replicated) float DamageMultiplier = 1;
	UPROPERTY(Replicated) float FireRate = 0;

	UPROPERTY() UAbilityHandlerSubSystem* AbilitiesSubSystem;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void Init(UAbilityHandlerSubSystem* abilitySys);
	
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetKnockBack() {return KnockBackMultiplier;}
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetDamageMultiplier() {return DamageMultiplier;}
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetFireRateMultiplier() {return FireRate;}
	
};

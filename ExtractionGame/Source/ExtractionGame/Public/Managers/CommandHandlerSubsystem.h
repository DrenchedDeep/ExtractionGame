

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommandHandlerSubsystem.generated.h"

class UAbilityHandlerSubSystem;
/**
 * 
 */

//Must be multicast for blueprints :(
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeOfDayChanged, float, time);

UCLASS()
class EXTRACTIONGAME_API UCommandHandlerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//WORLD	
	UFUNCTION(Exec) void SetTimeOfDay(float time) const;
	UFUNCTION(Exec) void SpawnActorOnSelf(FName ClassName) const;
	
	/// Spawns a gem
	/// @param type 0 (earth),1 (fire),2 (shadow),3 (water)
	/// @param polish gems polish (unlocked for funzies)
	UFUNCTION(Exec) void SpawnGemOnSelf(int type, float polish) const;
	
	UFUNCTION(Exec) void TPAllToMe(bool avoidObstacles = true, float spawnRadius = 1000 ) const;
	UFUNCTION(Exec) void TPAllToLocation(float x, float y, float z,bool avoidObstacles  = true, float spawnRadius = 1000) const;
	void TeleportAllPlayers(const AController* caller, const FVector& location, float radius, bool avoidObstacles) const;
	
	UFUNCTION(Exec) void EnableDebugInfo() const;
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
	UFUNCTION(Exec) void SetAttackCooldownMultiplier(float val);
	UFUNCTION(Exec) void SetCostMultiplier(float val);
	
	//SPECIFIC
	UFUNCTION(Exec) void SetAbilityDamage(int E, int F, int S, int W, float val);
	UFUNCTION(Exec) void SetAbilityFireRate(int E, int F, int S, int W, float val);
	UFUNCTION(Exec) void SetAbilityCost(int E, int F, int S, int W, float val) const;

	
	UFUNCTION(Exec) void GiveEssence(float amount) const;
	UFUNCTION(Exec) void ClearSaveData() const;

	UFUNCTION(Server, Reliable, WithValidation) void SetAbilityDamageServer(int idx, float val);
	UFUNCTION(Server, Reliable, WithValidation) void SetAbilityFireRateServer(int idx, float val);
	UFUNCTION(Server, Reliable, WithValidation) void SetAbilityCostServer(int idx, float val);
	UFUNCTION(Server, Reliable) void KillSelfServer(APlayerController* target);


	
	
	
protected:
	UPROPERTY(Replicated) float KnockBackMultiplier = 450;
	UPROPERTY(Replicated) float DamageMultiplier = 1;
	UPROPERTY(Replicated) float AttackCooldown = 1;

	UPROPERTY() UAbilityHandlerSubSystem* AbilitiesSubSystem;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnTimeOfDayChanged OnTimeOfDayChanged;
	
	void Init(UAbilityHandlerSubSystem* abilitySys);
	
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetKnockBack() {return KnockBackMultiplier;}
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetDamageMultiplier() {return DamageMultiplier;}
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetAttackCooldownMultiplier() {return AttackCooldown;}
	
};

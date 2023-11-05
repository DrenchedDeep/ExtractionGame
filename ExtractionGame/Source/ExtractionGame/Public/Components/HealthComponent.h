

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float MaxHealth = 100.f;

	bool bCanTakeDamage;

public:	
	UHealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(float Damage, const AController* Instigator);
	
	virtual void OnDeath(const FName& PlayerName);
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IsDead)
	bool bIsDead;

	UFUNCTION()
	virtual void OnRep_CurrentHealth();
	UFUNCTION()
	virtual void OnRep_IsDead();
	
	float GetCurrentHealth() const { return CurrentHealth; }

protected:
	virtual void BeginPlay() override;
};

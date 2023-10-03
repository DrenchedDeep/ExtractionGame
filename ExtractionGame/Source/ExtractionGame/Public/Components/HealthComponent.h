

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	float Damage;
	FVector Dir;
	FVector Origin;

	FDamageData(float Damage, const FVector& Dir, const FVector& Origin)
		: Damage(Damage),
		  Dir(Dir),
		  Origin(Origin)
	{
	}

	FDamageData() = default;
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float MaxHealth = 100.f;
	

public:	
	UHealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//runs on server
	virtual void ApplyDamage(FDamageData DamageInfo, bool bClientSimulation = false);
	//runs on server
	virtual void OnDeath();
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;
	
	UFUNCTION()
	virtual void OnRep_CurrentHealth();

	float GetCurrentHealth() const { return CurrentHealth; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

USTRUCT()
struct FDamageInfo
{
	GENERATED_BODY()
	
	FVector Origin;
	FVector Dir;
	float Damage;

	FDamageInfo(const FVector& Origin, const FVector& Dir, float Damage)
		: Origin(Origin),
		  Dir(Dir),
		  Damage(Damage)
	{
	}

	FDamageInfo() = default;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;
	
	
public:	
	UPlayerHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void ApplyDamage(FDamageInfo DamageInfo);
	
	UFUNCTION(Client, Reliable)
	void Client_DamageReceived();


	float GetCurrentHealth() const { return CurrentHealth; }
};

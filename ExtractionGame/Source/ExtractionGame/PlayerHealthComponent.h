#pragma once

#include "CoreMinimal.h"
#include "ExtractionGameCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/ActorComponent.h"
#include "Player/GemPlayerState.h"
#include "PlayerHealthComponent.generated.h"

class AExtractionGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Transient) AExtractionGameCharacter* Character;

	UPROPERTY(ReplicatedUsing=OnRep_IsDead)
	bool bIsDead;
	UPROPERTY(ReplicatedUsing=OnRep_HitCounter)
	int32 HitCount;

	bool bCanTakeDamage = true;
	
public:	
	UPlayerHealthComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnDeath(const FString& PlayerName);

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(float Damage, const AController* Instigator);

	float GetCurrentHealth() const
	{
		if(Character && Character->GemPlayerState)
		{
			return Character->GemPlayerState->GetHealth();
		}

		return 0.0f;
	}

	UFUNCTION(Client, Unreliable)
	void Client_ApplyDamage();
	UFUNCTION()
	virtual void OnRep_IsDead();
	UFUNCTION()
	virtual void OnRep_HitCounter();
	
};

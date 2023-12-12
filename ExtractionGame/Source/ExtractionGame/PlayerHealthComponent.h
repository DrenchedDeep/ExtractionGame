#pragma once

#include "CoreMinimal.h"
#include "ExtractionGameCharacter.h"
#include "Components/ActorComponent.h"
#include "UI/PlayerBarData.h"
#include "PlayerHealthComponent.generated.h"

class AExtractionGameHUD;
class AExtractionGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Transient) AExtractionGameCharacter* Character;
	UPROPERTY(Transient)UPlayerBarData* PlayerBarsWidget;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsDead) // this is usually done with a tag...
	bool bIsDead;
	UPROPERTY(ReplicatedUsing=OnRep_HitCounter)
	int32 HitCount;

	bool bCanTakeDamage = true;

	FDelegateHandle OnHealthChangedHandle;
	FDelegateHandle OnMaxHealthChangedHandle;
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	UPlayerBarData* GetHUDElement();
	void ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const;
public:
	
	float GetHealth() const;

	float GetMaxHealth() const;

	void SetHealth(float Health, const AController* Instigator);
	
	UPlayerHealthComponent();
	void Initialize();

	//virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnDeath(const FString& PlayerName);

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(float Damage, const AController* Instigator);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> HealthRegenEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> HealthPoolEffect;

	FActiveGameplayEffectHandle HealthRegenHandle;
	FActiveGameplayEffectHandle HealthPoolHandle;


	UFUNCTION(Client, Unreliable)
	void Client_ApplyDamage();
	UFUNCTION()
	virtual void OnRep_IsDead();
	UFUNCTION()
	virtual void OnRep_HitCounter();
	void Initialize(const AExtractionGameHUD* hud);

};

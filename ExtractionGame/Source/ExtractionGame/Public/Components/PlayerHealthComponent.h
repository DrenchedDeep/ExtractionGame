#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Components/ActorComponent.h"
#include "UI/Widgets/PlayerBarDataWidget.h"
#include "PlayerHealthComponent.generated.h"

class AExtractionGameHUD;
class AExtractionGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Transient) AExtractionGameCharacter* Character;
	UPROPERTY(Transient)UPlayerBarDataWidget* PlayerBarsWidget;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsDead) // this is usually done with a tag...
	bool bIsDead;

	bool bCanTakeDamage = true;

	FDelegateHandle OnHealthChangedHandle;
	FDelegateHandle OnMaxHealthChangedHandle;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	UPlayerBarDataWidget* GetHUDElement();
	void ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const;

	
public:
	
	float GetHealth() const;

	float GetMaxHealth() const;

	void SetHealth(float Health, const AController* Instigator);
	
	UPlayerHealthComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnDeath(const FString& PlayerName);

	UFUNCTION(BlueprintCallable)
	virtual bool ApplyDamage(float Damage, AController* Instigator);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> HealthRegenEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> HealthPoolEffect;

	FActiveGameplayEffectHandle HealthRegenHandle;
	FActiveGameplayEffectHandle HealthPoolHandle;


	UFUNCTION(Client, Unreliable)
	void Client_ApplyDamage(FVector FromDirection);
	UFUNCTION()
	virtual void OnRep_IsDead();
	
	void Initialize(const AExtractionGameHUD* hud);

};



#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Components/ExtractionAbilitySystemComponent.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "GameFramework/PlayerState.h"
#include "UI/PlayerBarData.h"
#include "GemPlayerState.generated.h"

/**
 * Player state inspired from unoffical GAS documentation.
 */
UCLASS()
class EXTRACTIONGAME_API AGemPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGemPlayerState();
	UPROPERTY()
	UPlayerBarData* GameHUD;
protected:

	UPROPERTY()
	UExtractionAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	UExtractionAttributeSet* AttributeSetBase;

	UPlayerBarData* GetUIElement();
	

	//FGameplayTag DeadTag;

	FDelegateHandle OnHealthChangedHandle;
	FDelegateHandle OnMaxHealthChangedHandle;
	
	FDelegateHandle OnEarthManaChangedHandle;
	FDelegateHandle OnFireManaChangedHandle;
	FDelegateHandle OnWaterManaChangedHandle;
	FDelegateHandle OnShadowManaChangedHandle;

	FDelegateHandle OnMaxEarthManaChangedHandle;
	FDelegateHandle OnMaxFireManaChangedHandle;
	FDelegateHandle OnMaxWaterManaChangedHandle;
	FDelegateHandle OnMaxShadowManaChangedHandle;

	//UFUNCTION(BlueprintImplementableEvent)
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnEarthManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxEarthManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnFireManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxFireManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnShadowManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxShadowManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnWaterManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxWaterManaChanged(const FOnAttributeChangeData& Data);
	

public:
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UExtractionAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetEarthMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetEarthMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetFireMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetFireMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetShadowMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetShadowMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetWaterMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetWaterMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetManaRegenRate() const;

	void SetHealth(float Health);
	void CreateStateFuncs();
};

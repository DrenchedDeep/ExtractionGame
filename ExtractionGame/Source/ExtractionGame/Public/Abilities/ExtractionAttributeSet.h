// © 2021, Matthew Barham. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ExtractionAttributeSet.generated.h"

// Uses macros from Attributes.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Hopper Attribute Set
 */
UCLASS()
class EXTRACTIONGAME_API UExtractionAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UExtractionAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	/** MaxHealth is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxHealth)
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_RegenMana)
	FGameplayAttributeData RegenHealth;
	ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, RegenHealth)
	UFUNCTION()
	virtual void OnRep_RegenHealth(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly, Category = "Speed", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, Speed)
	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldValue);

	/** GemMana Pools **/
#pragma region Mana
	
	UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_RegenMana)
	FGameplayAttributeData RegenMana;

	
	ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, RegenMana);
	UFUNCTION()
	virtual void OnRep_RegenMana(const FGameplayAttributeData& OldValue);

	#pragma region Earth
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_EarthMana)
		FGameplayAttributeData EarthManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, EarthManaPool);
		UFUNCTION()
		virtual void OnRep_EarthMana(const FGameplayAttributeData& OldValue);

		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_MaxEarthMana)
		FGameplayAttributeData MaxEarthManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxEarthManaPool);
		UFUNCTION()
		virtual void OnRep_MaxEarthMana(const FGameplayAttributeData& OldValue);
	# pragma endregion Earth
	#pragma region Fire
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_FireMana)
		FGameplayAttributeData FireManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, FireManaPool);
		UFUNCTION()
		virtual void OnRep_FireMana(const FGameplayAttributeData& OldValue);

		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_MaxFireMana)
		FGameplayAttributeData MaxFireManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxFireManaPool);
		UFUNCTION()
		virtual void OnRep_MaxFireMana(const FGameplayAttributeData& OldValue);
	# pragma endregion Fire
	#pragma region Shadow
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_ShadowMana)
		FGameplayAttributeData ShadowManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, ShadowManaPool);
		UFUNCTION()
		virtual void OnRep_ShadowMana(const FGameplayAttributeData& OldValue);

		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_MaxShadowMana)
		FGameplayAttributeData MaxShadowManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxShadowManaPool);
		UFUNCTION()
		virtual void OnRep_MaxShadowMana(const FGameplayAttributeData& OldValue);
	# pragma endregion Shadow
	#pragma region Water
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_WaterMana)
		FGameplayAttributeData WaterManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, WaterManaPool);
		UFUNCTION()
		virtual void OnRep_WaterMana(const FGameplayAttributeData& OldValue);
		
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_MaxWaterMana)
		FGameplayAttributeData MaxWaterManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxWaterManaPool);
		UFUNCTION()
		virtual void OnRep_MaxWaterMana(const FGameplayAttributeData& OldValue);
	# pragma endregion Water
# pragma endregion Mana
	/**
	 * Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	 * (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	 */
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	                                 const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	                                 const FGameplayAttribute& AffectedAttributeProperty) const;



};
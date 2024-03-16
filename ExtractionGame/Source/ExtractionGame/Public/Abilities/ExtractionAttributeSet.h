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


	UPROPERTY(BlueprintReadOnly, Category = "Essence", ReplicatedUsing = OnRep_Essence)
	FGameplayAttributeData Essence;
	ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, Essence);
	UFUNCTION()
	virtual void OnRep_Essence(const FGameplayAttributeData& OldValue);


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

	#pragma region Left
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_LeftMana)
		FGameplayAttributeData LeftManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, LeftManaPool);
		UFUNCTION()
		virtual void OnRep_LeftMana(const FGameplayAttributeData& OldValue);

		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_MaxLeftMana)
		FGameplayAttributeData MaxLeftManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxLeftManaPool);
		UFUNCTION()
		virtual void OnRep_MaxLeftMana(const FGameplayAttributeData& OldValue);
	# pragma endregion Left
	#pragma region Right
		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_RightMana)
		FGameplayAttributeData RightManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, RightManaPool);
		UFUNCTION()
		virtual void OnRep_RightMana(const FGameplayAttributeData& OldValue);

		UPROPERTY(BlueprintReadOnly, Category = "GemMana", ReplicatedUsing = OnRep_MaxRightMana)
		FGameplayAttributeData MaxRightManaPool;
		ATTRIBUTE_ACCESSORS(UExtractionAttributeSet, MaxRightManaPool);
		UFUNCTION()
		virtual void OnRep_MaxRightMana(const FGameplayAttributeData& OldValue);
	# pragma endregion Right
# pragma endregion Mana
	/**
	 * Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	 * (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	 */
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	                                 const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	                                 const FGameplayAttribute& AffectedAttributeProperty) const;



};
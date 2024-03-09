#include "Abilities/ExtractionAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Net/UnrealNetwork.h"

UExtractionAttributeSet::UExtractionAttributeSet()
{
}

void UExtractionAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, RegenMana, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, RegenHealth, COND_None, REPNOTIFY_OnChanged);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, LeftManaPool, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, RightManaPool, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, MaxLeftManaPool, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UExtractionAttributeSet, MaxRightManaPool, COND_None, REPNOTIFY_OnChanged);
}

void UExtractionAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{\
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	// If a Max value changes, adjust current to keep Current % of Current to Max
	else if (Attribute == GetMaxLeftManaPoolAttribute())
	{
		AdjustAttributeForMaxChange(LeftManaPool, MaxLeftManaPool, NewValue, GetLeftManaPoolAttribute());
	}
	else if (Attribute == GetMaxRightManaPoolAttribute())
	{
		AdjustAttributeForMaxChange(RightManaPool, MaxRightManaPool, NewValue, GetRightManaPoolAttribute());
	}
}

void UExtractionAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	/*
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AExtractionGameCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AExtractionGameCharacter>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AExtractionGameCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<AExtractionGameCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AExtractionGameCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	} 
	*/
	//Mana increases based on FCurve.
	//Max Mana is based on gems
	//
	
	if(Data.EvaluatedData.Attribute == GetLeftManaPoolAttribute())
	{
		SetLeftManaPool(FMath::Clamp(GetLeftManaPool(), 0.f, GetMaxLeftManaPool()));
	}
	else if(Data.EvaluatedData.Attribute == GetRightManaPoolAttribute())
	{
		SetRightManaPool(FMath::Clamp(GetRightManaPool(), 0.f, GetMaxRightManaPool()));
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(GetMaxHealth());
	}
	
}


void UExtractionAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
                                                          const FGameplayAttributeData& MaxAttribute,
                                                          const float NewMaxValue,
                                                          const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f)? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue: NewMaxValue;
		UE_LOG(LogTemp, Warning, TEXT("Setting ability from %f to %f"), CurrentValue, CurrentValue+NewDelta)
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}


void UExtractionAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, Health, OldValue);
}

void UExtractionAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, MaxHealth, OldValue);
}

void UExtractionAttributeSet::OnRep_RegenHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, RegenHealth, OldValue);
}

void UExtractionAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, Speed, OldValue);
}

void UExtractionAttributeSet::OnRep_RegenMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, RegenMana, OldValue);
}

void UExtractionAttributeSet::OnRep_LeftMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, LeftManaPool, OldValue);
}

void UExtractionAttributeSet::OnRep_MaxLeftMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, MaxLeftManaPool, OldValue);
}


void UExtractionAttributeSet::OnRep_RightMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, RightManaPool, OldValue);
}

void UExtractionAttributeSet::OnRep_MaxRightMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExtractionAttributeSet, MaxRightManaPool, OldValue);
}

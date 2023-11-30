

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ExtractionAttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionOnAttribChanged.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);


UCLASS()
class EXTRACTIONGAME_API UAsyncActionOnAttribChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor;
	TArray<FGameplayAttribute> AttributesToListenFor;

	void AttributeChanged(const FOnAttributeChangeData& Data) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;
	
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
	// Listens for an attribute changing.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncActionOnAttribChanged* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);

};

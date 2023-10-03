

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilityHandlerSubSystem.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UAbilityHandlerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	TMap<int32, TSubclassOf<UGameplayAbility>> ObjectMap;
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayAbility> GetAbilityByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	int ConvertToIntID(int Earth, int Fire, int Shadow, int Water);
	

	UFUNCTION(BlueprintCallable, Category = "MySubsystem")
	void AddAbilityToMap(int32 Index, TSubclassOf<UGameplayAbility> Object);

};

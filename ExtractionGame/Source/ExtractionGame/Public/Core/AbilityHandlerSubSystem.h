

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
	int32 ConvertToIntID(int32 Earth, int32 Fire, int32 Shadow, int32 Water);


	UFUNCTION(BlueprintCallable, Category = "MySubsystem")
	void AddAbilityToMap(int32 Index, TSubclassOf<UGameplayAbility> Object);

};

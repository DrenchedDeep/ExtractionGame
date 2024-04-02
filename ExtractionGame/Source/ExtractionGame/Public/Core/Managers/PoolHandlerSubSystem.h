

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityObjectPool.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PoolHandlerSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UPoolHandlerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UPROPERTY()
	AAbilityObjectPool* Pool;
public:
	UFUNCTION(BlueprintCallable)
	APooledObject* SpawnObject(FString type);
	
	void SetPool(AAbilityObjectPool* newPool);
	AAbilityObjectPool* GetPool() const {return Pool;}
	
	
};

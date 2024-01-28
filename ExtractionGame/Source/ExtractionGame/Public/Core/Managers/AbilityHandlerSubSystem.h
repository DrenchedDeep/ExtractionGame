

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

	UPROPERTY()
	TMap<int32, UTexture2D*> IconsMap;
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayAbility> GetAbilityByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetGemSprite(const AGem* gem);

	UFUNCTION(BlueprintCallable, Category = "MySubsystem")
	void AddGemSpriteToMap(int32 Index, UTexture2D* Object);

	UFUNCTION(BlueprintCallable)
	int ConvertToIntID(int32 Earth, int32 Fire, int32 Shadow, int32 Water);


	UFUNCTION(BlueprintCallable, Category = "MySubsystem")
	void AddAbilityToMap(int32 Index, TSubclassOf<UGameplayAbility> Object);

	bool ContainsAbility(int32 ability) const;

};

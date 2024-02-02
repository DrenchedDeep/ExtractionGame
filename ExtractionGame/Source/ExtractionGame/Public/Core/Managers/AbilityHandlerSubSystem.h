#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilityHandlerSubSystem.generated.h"


/**
 * MyStruct definition
 */
USTRUCT(BlueprintType)
struct FAbilityStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	bool bIsFullyAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	TSubclassOf<class UGameplayAbility> GameplayAbilityClass;
};

/**
 * AbilityHandlerSubSystem class
 */
UCLASS()
class EXTRACTIONGAME_API UAbilityHandlerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	TMap<int32, FAbilityStruct> ObjectMap; // Use the struct instead of TSubclassOf<UGameplayAbility>

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	FAbilityStruct GetAbilityByIndex(int32 Index); // Change the return type to the struct

	//UFUNCTION(BlueprintCallable)
	//UTexture2D* GetGemSprite(const AGem* gem);

	//UFUNCTION(BlueprintCallable, Category = "MySubsystem")
	//void AddGemSpriteToMap(int32 Index, UTexture2D* Object);

	UFUNCTION(BlueprintCallable)
	int ConvertToIntID(int32 Earth, int32 Fire, int32 Shadow, int32 Water);

	UFUNCTION(BlueprintCallable, Category = "MySubsystem")
	void AddAbilityToMap(int32 Index, FAbilityStruct Object); // Change the parameter type to the struct

	bool ContainsAbility(int32 ability) const;
};

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilityHandlerSubSystem.generated.h"


class UGameplayEffect;
class UGameplayAbility;
/**
 * MyStruct definition
 */

UENUM(BlueprintType)
enum EAbilityAnimType
{
	EA_None,
	EA_Punch,
	EA_Lob,
	EA_Throw
};
USTRUCT(BlueprintType)
struct FAbilityStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	bool bIsFullyAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	FString AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	FString AbilityName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyStruct")
	TEnumAsByte<EAbilityAnimType> AnimType;
};

/**
 * AbilityHandlerSubSystem class
 */
UCLASS()
class EXTRACTIONGAME_API UAbilityHandlerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	TMap<int32, FAbilityStruct> ObjectMap; // Use the struct instead of TSubclassOf<UGameplayAbility>
	TMap<int32, TSubclassOf<UGameplayEffect>> EffectMap; // Use the struct instead of TSubclassOf<UGameplayAbility>

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable) FAbilityStruct GetAbilityByIndex(int32 Index); // Change the return type to the struct
	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayEffect> GetEffectByIndex(int32 Index); // Change the return type to the struct

	UFUNCTION(BlueprintCallable) int ConvertToIntID(int32 Earth, int32 Fire, int32 Shadow, int32 Water);

	UFUNCTION(BlueprintCallable, Category = "MySubsystem") void AddAbilityToMap(int32 Index, FAbilityStruct Object); // Change the parameter type to the struct
	UFUNCTION(BlueprintCallable, Category = "MySubsystem") void AddEffectToMap(int32 Index, TSubclassOf<UGameplayEffect> Object); // Change the parameter type to the struct

	bool ContainsAbility(int32 ability) const;
	bool ContainsEffect(int32 ability) const;
};

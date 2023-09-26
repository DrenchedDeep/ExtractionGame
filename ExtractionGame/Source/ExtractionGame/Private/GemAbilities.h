

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GemAbilities.generated.h"

/**
 * 
 */

UCLASS()
class UGemAbilities : public UBlueprintFunctionLibrary
{
	
public:
	typedef static void (*AbilityPtr)(float);
private:

	
	GENERATED_BODY()

	static TMap<int, AbilityPtr> abilities;

	
	UFUNCTION(BlueprintPure)
	static void FireBolt(float pow);
	
	UFUNCTION(BlueprintPure)
	static void WaterStream(float pow);

	UFUNCTION(BlueprintPure)
	static void SmokeSpray(float pow);
	
public:
	

	
	static int convertToInteger(int fire, int water, int light, int dark);

	
	


	static AbilityPtr* FindAbility(int GemCombo);

	UGemAbilities();
};


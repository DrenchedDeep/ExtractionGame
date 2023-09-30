

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
private:

	
	GENERATED_BODY()

	
	static void FireBolt(float pow);
	
	static void WaterStream(float pow);

	static void SmokeSpray(float pow);
	
public:
	

	
	static int convertToInteger(int fire, int water, int light, int dark);

	
	
	
	UGemAbilities();
};


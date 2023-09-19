

#pragma once

#include "CoreMinimal.h"
#include "GemController.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GemAbilities.generated.h"

/**
 * 
 */

UCLASS()
class UGemAbilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	UFUNCTION(Reliable, NetMulticast, BlueprintSetter)
	void FireBolt();
	
	UFUNCTION(Reliable, NetMulticast, BlueprintSetter)
	void WaterJet();

	UFUNCTION(Reliable, NetMulticast, BlueprintSetter)
	void SmokeSpray();
	*/
	
	
	UPROPERTY(Category = "Abilities", EditAnywhere, BlueprintReadWrite)
	TMap<int, UFunction*> abilities;


	static FAttackSignature FindAbility(int GemCombo, float TotalPolish);
};

